#pragma once

#include "stm32f103xb.h"
#include <stdint.h>
#include "GPIO.hpp"

#ifdef __cplusplus
extern "C"{
#endif
extern void TIM1_IRQHandler(void);
extern void TIM2_IRQHandler(void);
extern void TIM3_IRQHandler(void);
extern void TIM4_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#define AFIO_MAPR_TIM1_REMAP_0_Pos  AFIO_MAPR_TIM1_REMAP_Pos
#define AFIO_MAPR_TIM1_REMAP_0_Msk  AFIO_MAPR_TIM1_REMAP_Msk
#define AFIO_MAPR_TIM1_REMAP_1_Pos  (AFIO_MAPR_TIM1_REMAP_Pos<<1)
#define AFIO_MAPR_TIM1_REMAP_1_Msk  (AFIO_MAPR_TIM1_REMAP_Msk<<1)
#define AFIO_MAPR_TIM2_REMAP_0_Pos  AFIO_MAPR_TIM2_REMAP_Pos
#define AFIO_MAPR_TIM2_REMAP_0_Msk  AFIO_MAPR_TIM2_REMAP_Msk
#define AFIO_MAPR_TIM2_REMAP_1_Pos  (AFIO_MAPR_TIM2_REMAP_Pos<<1)
#define AFIO_MAPR_TIM2_REMAP_1_Msk  (AFIO_MAPR_TIM2_REMAP_Msk<<1)

class TIM
{
public:
    enum Mode
    {
        TIMER, PWMOUT, COMP_PWMOUT, ENCODER
    };
    TIM(){};
    ~TIM(){};
    void enableClock(void);
    void disableClock(void);
    bool isEnable(void);
    void enableCount(void);
    void disableCount(void);
    void setCount(uint16_t count);
    uint16_t getCount(void);
    void resetCount(void);
    bool isCountEnable(void);

    void setupPwmOut(TIM_TypeDef* tim,
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
        GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
        GPIO_TypeDef* gpioCh4, uint8_t pinCh4);
    void setupTimer(TIM_TypeDef* tim, uint16_t prescaler, uint32_t interruptTime);

    void setupEncoder(TIM_TypeDef* tim, GPIO_TypeDef* gpioA, uint8_t pinA, GPIO_TypeDef* gpioB, uint8_t pinB, uint16_t period);

    class Channel
    {
    public:
        Channel(){};
        ~Channel(){};
        Channel(GPIO_TypeDef* gpio, uint8_t pin, GPIO::Mode mode);

        //ピンにPWMを出力
        //クラス外の設定PWMにアクセスする必要
        void pwm();

    private:
        GPIO* m_gpio;
    };

    Channel* ch1;
    Channel* ch1n;
    Channel* ch2;
    Channel* ch2n;
    Channel* ch3;
    Channel* ch3n;
    Channel* ch4;
private:
    TIM_TypeDef* TIMx;
    Mode m_mode;
    void setup(TIM_TypeDef* tim, Mode mode);

    //1ピンずつのｒｅｍａｐ?
    void remap(
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2);
    void remap(
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
        GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
        GPIO_TypeDef* gpioCh4, uint8_t pinCh4);
    void remapComp(
        GPIO_TypeDef* gpioCh1, uint8_t pinCh1,
        GPIO_TypeDef* gpioCh1n, uint8_t pinCh1n,
        GPIO_TypeDef* gpioCh2, uint8_t pinCh2,
        GPIO_TypeDef* gpioCh2n, uint8_t pinCh2n,
        GPIO_TypeDef* gpioCh3, uint8_t pinCh3,
        GPIO_TypeDef* gpioCh3n, uint8_t pinCh3n,
        GPIO_TypeDef* gpioCh4, uint8_t pinCh4);

};
