#include "GPIO.hpp"

//PERIPH->REGにのビットをVALにする
#define writeBit(PERIPH, REG, SYM, VAL)   (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)
#define readBit(PERIPH, REG, SYM)         (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg()

//extern void setAlternate_CAN();
void setAlternate_JTAG_SWD(JTAG_SWD_MODE mode);

GPIO::GPIO()
{

}

GPIO::GPIO(GPIO_TypeDef* gpio, uint8_t pin, Mode mode)
{
    setup(gpio, pin, mode);
}

GPIO::~GPIO()
{
    //TODO:デストラクタの作成
    //ピン0に設定
    //AFIOデフォルトに設定
}

void GPIO::setup(GPIO_TypeDef* gpio, uint8_t pin, Mode mode)
{
    if(pin < 0 || pin > 15) return;
    
    this->GPIOx = gpio;
    this->pin = pin;
    
    //clock enable
    if(GPIOx == GPIOA) writeBit(RCC, APB2ENR, IOPAEN, 1);
    if(GPIOx == GPIOB) writeBit(RCC, APB2ENR, IOPBEN, 1);
    if(GPIOx == GPIOC) writeBit(RCC, APB2ENR, IOPCEN, 1);
    if(GPIOx == GPIOD) writeBit(RCC, APB2ENR, IOPDEN, 1);
    if(GPIOx == GPIOE) writeBit(RCC, APB2ENR, IOPEEN, 1);
    if((mode & 0x6) == 0x6) writeBit(RCC, APB2ENR, AFIOEN, 1);

    uint8_t filter = (~(mode >> 1) | (0x2 & mode));
    if(pin < 8)
    {
        //一回0にリセット
        GPIOx->CRL &= ~(0xF << pin*4);
        switch(mode)
        {
            //入力******************************
            case ANALOG:    //fall through
            case FLOATING:  //fall
            case INPUT_PD:  //fall
            case INPUT_PU:
                GPIOx->CRL |= (mode & filter) << ((pin*4)+2);
                break;
            //出力******************************
            case PUSHPULL:      //fall through
            case OPENDRAIN:     //fall
            case ALT_PUSHPULL:  //fall
            case ALT_OPENDRAIN:
                GPIOx->CRL |= (0x3 << pin*4);    //50MHz出力
                GPIOx->CRL |= ((mode & 0x3) << (pin*4+2));
                break;
            default: break;
        }
    }
    else
    {
        GPIOx->CRH &= ~(0xF << (pin-8)*4);
        switch(mode)
        {
            //入力******************************
            case ANALOG:
            case FLOATING:
            case INPUT_PD:
            case INPUT_PU:
                GPIOx->CRH |= (mode & filter) << (((pin-8)*4)+2);
                break;
                
            //出力******************************
            case PUSHPULL:
            case OPENDRAIN:
            case ALT_PUSHPULL:
            case ALT_OPENDRAIN:
                GPIOx->CRH |= (0x3 << (pin-8)*4);    //50MHz出力
                GPIOx->CRH |= ((mode & 0x3) << ((pin-8)*4+2));
                break;
            default: break;
        }
    }
}

void setAlternate_CAN(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx)
{
    //一度リセット
    AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP;
    if(gpioRx == GPIOA && pinRx == 11 && gpioTx == GPIOA && pinTx == 12)
    {
        //00
        AFIO->MAPR |= (~AFIO_MAPR_CAN_REMAP_1 | ~AFIO_MAPR_CAN_REMAP_0);
    }
    if(gpioRx == GPIOB && pinRx == 8 && gpioTx == GPIOB && pinTx == 9)
    {
        //10
        AFIO->MAPR |= (AFIO_MAPR_CAN_REMAP_1 | ~AFIO_MAPR_CAN_REMAP_0);
    }
    if(gpioRx == GPIOD && pinRx == 0 && gpioTx == GPIOD && pinTx == 1)
    {
        //11
        AFIO->MAPR |= (AFIO_MAPR_CAN_REMAP_1 | AFIO_MAPR_CAN_REMAP_0);
    }
}

void setAlternate_JTAG_SWD(JTAG_SWD_MODE mode)
{
    //一度リセット
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= mode & 0x7;
}

uint8_t GPIO::read(void)
{
    return (uint8_t)((GPIOx->IDR >> pin) & 0x01);
}

void GPIO::write(uint8_t value)
{
    if(value)
        GPIOx->ODR |= (0x1 << pin);
    else
        GPIOx->ODR &= ~(0x1 << pin);
}

void GPIO::toggle(void)
{
    if(read()) write(0);
    else write(1);
}
