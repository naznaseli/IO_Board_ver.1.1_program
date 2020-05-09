#include "TIM.hpp"
#include <stdio.h>
#include "RCC.hpp"

#define writeBit(PERIPH, REG, SYM, VAL) (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)      (PERIPH->REG)
#define readBit(PERIPH, REG, SYM)       (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg(PERIPH, REG)  ()

extern void interrupt(void);

void TIM1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

void TIM1_IRQHandler(void)
{
    TIM1->SR = 0;
}

void TIM2_IRQHandler(void)
{
    TIM2->SR = 0;
}

void TIM3_IRQHandler(void)
{
    TIM3->SR = 0;
    interrupt();
}

void TIM4_IRQHandler(void)
{
    TIM4->SR = 0;
}

void TIM::setup(TIM_TypeDef* tim, Mode mode)
{
    TIMx = tim;
    m_mode = mode;

    if(TIMx == TIM1)
    {
        writeBit(RCC, APB2ENR, TIM1EN, 1);
        //NVIC_EnableIRQ(TIM2_IRQn);
    }
    if(TIMx == TIM2)
    {
        writeBit(RCC, APB1ENR, TIM2EN, 1);
        NVIC_EnableIRQ(TIM2_IRQn);
    }
    if(TIMx == TIM3)
    {
        writeBit(RCC, APB1ENR, TIM3EN, 1);
        NVIC_EnableIRQ(TIM3_IRQn);
    }
    if(TIMx == TIM4)
    {
        writeBit(RCC, APB1ENR, TIM4EN, 1);
        NVIC_EnableIRQ(TIM4_IRQn);
    }
}

    //interruptTime = (ARR * PSC) / TIMXCLK [us]
    //interruptTime = (ARR * PSC) / TIMXCLK / 10^3 [ms]
void TIM::setupTimer(TIM_TypeDef* tim, uint16_t prescaler, uint32_t interruptTime)
{
    setup(tim, TIMER);

    //更新割り込みリクエスト許可
    TIMx->DIER = TIM_DIER_UIE;

    TIMx->PSC = prescaler - 1;
    TIMx->ARR = (TIMXCLK * interruptTime) / (prescaler * 1000) - 1;
    resetCount();
    //enableCount();
}

//なければNULLと-1入れる
void TIM::setupPwmOut(TIM_TypeDef* tim,
    GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
    GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
    GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
    GPIO_TypeDef* gpioCh4, uint8_t pinCh4)
{
    setup(tim, PWMOUT);
    if(gpioCh1 != NULL)
        ch1 = new Channel(gpioCh1, pinCh1, GPIO::ALT_OPENDRAIN);
    if(gpioCh2 != NULL)
        ch2 = new Channel(gpioCh2, pinCh2, GPIO::ALT_OPENDRAIN);
    if(gpioCh3 != NULL)
        ch1 = new Channel(gpioCh3, pinCh3, GPIO::ALT_OPENDRAIN);
    if(gpioCh4 != NULL)
        ch1 = new Channel(gpioCh4, pinCh4, GPIO::ALT_OPENDRAIN);

    //TIMクラス関数
    remap(gpioCh1, pinCh1, gpioCh2, pinCh2, gpioCh3, pinCh3, gpioCh4, pinCh4);

    //pwm出力の設定レジスタいじり

    //TIMクラス設定
    resetCount();
    enableCount();

}

void TIM::setupEncoder(TIM_TypeDef* tim, GPIO_TypeDef* gpioA, uint8_t pinA, GPIO_TypeDef* gpioB, uint8_t pinB, uint16_t period)
{
    setup(tim, ENCODER);
    ch1 = new Channel(gpioA, pinA, GPIO::FLOATING);
    ch2 = new Channel(gpioB, pinB, GPIO::FLOATING);
    remap(gpioA, pinA, gpioB, pinB, NULL, -1, NULL, -1);

    TIMx->SMCR |= 0x0003;   //エンコーダモード3
    TIMx->CCMR1 |= 0x0101;
    TIMx->CR1 |= TIM_CR1_ARPE;
    TIMx->ARR = period;

    resetCount();
    enableCount();
}

void TIM::remap(
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
        GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
        GPIO_TypeDef* gpioCh4, uint8_t pinCh4)
{
    if(TIMx == TIM1)
    {
        if((gpioCh1 == GPIOA && pinCh1 == 8) || (gpioCh2 == GPIOA && pinCh2 == 9))
            writeBit(AFIO, MAPR, TIM1_REMAP_1, 0);
        if((gpioCh1 == GPIOE && pinCh1 == 9) && (gpioCh2 == GPIOE && pinCh2 == 11))
            writeBit(AFIO, MAPR, TIM1_REMAP_1, 1);
    }
    if(TIMx == TIM2)
    {
        if((gpioCh1 == GPIOA && pinCh1 == 0) || (gpioCh2 == GPIOA && pinCh2 == 1))
            writeBit(AFIO, MAPR, TIM2_REMAP_0, 0);
        if((gpioCh1 == GPIOA && pinCh1 == 15) || (gpioCh2 == GPIOB && pinCh2 == 3))
            writeBit(AFIO, MAPR, TIM2_REMAP_0, 1);
    }
    //tim3, tim4 未完
    if(TIMx == TIM3)
    {
        if((gpioCh1 == GPIOA && pinCh1 == 6) || (gpioCh2 == GPIOA && pinCh2 == 7))
            writeBit(AFIO, MAPR, TIM3_REMAP, 0);    //再割り当てなし
        if((gpioCh1 == GPIOB && pinCh1 == 4) || (gpioCh2 == GPIOB && pinCh2 == 5))
            writeBit(AFIO, MAPR, TIM3_REMAP, 1);    //部分的な再割り当て
        if((gpioCh1 == GPIOC && pinCh1 == 6) || (gpioCh2 == GPIOC && pinCh2 == 7))
            writeBit(AFIO, MAPR, TIM3_REMAP, 3);    //完全な再割り当て
    }
    if(TIMx == TIM4)
    {
        if((gpioCh1 == GPIOB && pinCh1 == 6) || (gpioCh2 == GPIOB && pinCh2 == 7))
            writeBit(AFIO, MAPR, TIM4_REMAP, 0);    //再割り当てなし
        if((gpioCh1 == GPIOD && pinCh1 == 12) || (gpioCh2 == GPIOD && pinCh2 == 13))
            writeBit(AFIO, MAPR, TIM4_REMAP, 1);    //部分的な再割り当て
    }
}

void TIM::remapComp(
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh1n, uint8_t pinCh1n,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
        GPIO_TypeDef* gpioCh2n, uint8_t pinCh2n,
        GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
        GPIO_TypeDef* gpioCh3n, uint8_t pinCh3n,
        GPIO_TypeDef* gpioCh4, uint8_t pinCh4)
{
}


void TIM::enableCount(void)
{
    TIMx->CR1 |= TIM_CR1_CEN;

}

uint16_t TIM::getCount(void)
{
    return TIMx->CNT;
}

void TIM::setCount(uint16_t count)
{
    TIMx->CNT = count;
}

void TIM::resetCount(void)
{
    setCount(0);
}

TIM::Channel::Channel(GPIO_TypeDef* gpio, uint8_t pin, GPIO::Mode mode)
{
    m_gpio = new GPIO(gpio, pin, mode);
}
