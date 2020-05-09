#pragma once

#include "stm32f103xb.h"

#define PA0     GPIOA,(uint8_t)0
#define PA1     GPIOA,(uint8_t)1
#define PA2     GPIOA,(uint8_t)2
#define PA3     GPIOA,(uint8_t)3
#define PA4     GPIOA,(uint8_t)4
#define PA5     GPIOA,(uint8_t)5
#define PA6     GPIOA,(uint8_t)6
#define PA7     GPIOA,(uint8_t)7
#define PA8     GPIOA,(uint8_t)8
#define PA9     GPIOA,(uint8_t)9
#define PA10    GPIOA,(uint8_t)10
#define PA11    GPIOA,(uint8_t)11
#define PA12    GPIOA,(uint8_t)12
#define PA13    GPIOA,(uint8_t)13
#define PA14    GPIOA,(uint8_t)14
#define PA15    GPIOA,(uint8_t)15

#define PB0     GPIOB,(uint8_t)0
#define PB1     GPIOB,(uint8_t)1
#define PB2     GPIOB,(uint8_t)2
#define PB3     GPIOB,(uint8_t)3
#define PB4     GPIOB,(uint8_t)4
#define PB5     GPIOB,(uint8_t)5
#define PB6     GPIOB,(uint8_t)6
#define PB7     GPIOB,(uint8_t)7
#define PB8     GPIOB,(uint8_t)8
#define PB9     GPIOB,(uint8_t)9
#define PB10    GPIOB,(uint8_t)10
#define PB11    GPIOB,(uint8_t)11
#define PB12    GPIOB,(uint8_t)12
#define PB13    GPIOB,(uint8_t)13
#define PB14    GPIOB,(uint8_t)14
#define PB15    GPIOB,(uint8_t)15

#define PC0     GPIOC,(uint8_t)0
#define PC1     GPIOC,(uint8_t)1
#define PC2     GPIOC,(uint8_t)2
#define PC3     GPIOC,(uint8_t)3
#define PC4     GPIOC,(uint8_t)4
#define PC5     GPIOC,(uint8_t)5
#define PC6     GPIOC,(uint8_t)6
#define PC7     GPIOC,(uint8_t)7
#define PC8     GPIOC,(uint8_t)8
#define PC9     GPIOC,(uint8_t)9
#define PC10    GPIOC,(uint8_t)10
#define PC11    GPIOC,(uint8_t)11
#define PC12    GPIOC,(uint8_t)12
#define PC13    GPIOC,(uint8_t)13
#define PC14    GPIOC,(uint8_t)14
#define PC15    GPIOC,(uint8_t)15

#define PD0     GPIOD,(uint8_t)0
#define PD1     GPIOD,(uint8_t)1
#define PD2     GPIOD,(uint8_t)2
#define PD3     GPIOD,(uint8_t)3
#define PD4     GPIOD,(uint8_t)4
#define PD5     GPIOD,(uint8_t)5
#define PD6     GPIOD,(uint8_t)6
#define PD7     GPIOD,(uint8_t)7
#define PD8     GPIOD,(uint8_t)8
#define PD9     GPIOD,(uint8_t)9
#define PD10    GPIOD,(uint8_t)10
#define PD11    GPIOD,(uint8_t)11
#define PD12    GPIOD,(uint8_t)12
#define PD13    GPIOD,(uint8_t)13
#define PD14    GPIOD,(uint8_t)14
#define PD15    GPIOD,(uint8_t)15

#define PE0     GPIOE,(uint8_t)0
#define PE1     GPIOE,(uint8_t)1
#define PE2     GPIOE,(uint8_t)2
#define PE3     GPIOE,(uint8_t)3
#define PE4     GPIOE,(uint8_t)4
#define PE5     GPIOE,(uint8_t)5
#define PE6     GPIOE,(uint8_t)6
#define PE7     GPIOE,(uint8_t)7
#define PE8     GPIOE,(uint8_t)8
#define PE9     GPIOE,(uint8_t)9
#define PE10    GPIOE,(uint8_t)10
#define PE11    GPIOE,(uint8_t)11
#define PE12    GPIOE,(uint8_t)12
#define PE13    GPIOE,(uint8_t)13
#define PE14    GPIOE,(uint8_t)14
#define PE15    GPIOE,(uint8_t)15

enum JTAG_SWD_MODE
{
    FULL_SWJ = 0x0, FULL_SWJ_NO_JNTRST = 0x1, SWDP = 0x2, DISABLE = 0x4
};

//TODO:setAlternate
extern void setAlternate_CAN(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);
extern void setAlternate_JTAG_SWD(JTAG_SWD_MODE mode);
extern void setAlternate_ADC(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);
extern void setAlternate_Timer(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);
extern void setAlternate_USART(
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx, 
        GPIO_TypeDef* gpioCk, uint8_t pinCk, 
        GPIO_TypeDef* gpioCts, uint8_t pinCts, 
        GPIO_TypeDef* gpioRts, uint8_t pinRts);
extern void setAlternate_I2C(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);
extern void setAlternate_SPI(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);

class GPIO
{
public:
    enum Mode
    {
        ANALOG  =   0x00,
        FLOATING =  0x01,
        INPUT_PD =  0x02,
        INPUT_PU =  0x03,
        PUSHPULL =  0x04,
        OPENDRAIN = 0x05,
        ALT_PUSHPULL = 0x06,
        ALT_OPENDRAIN = 0x07
    };
    
    GPIO();
    ~GPIO();
        
    GPIO(GPIO_TypeDef* gpio, uint8_t pin, Mode mode);
    void setup(GPIO_TypeDef* gpio, uint8_t pin, Mode mode);

    uint8_t read(void);
    void write(uint8_t value);
    void toggle(void);

    GPIO_TypeDef* GPIOx;
    uint8_t pin;
};
