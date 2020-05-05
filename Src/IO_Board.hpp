#pragma once

#include "./peripheral/RCC.hpp"
#include "./peripheral/GPIO.hpp"
#include "./peripheral/TIM.hpp"
#include "./peripheral/USART.hpp"
#include "./peripheral/bxCAN.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
//extern int __io_getchar(void);
//extern int __io_putchar(int ch);
#ifdef __cplusplus
}
#endif

//extern GPIO io[16];
//extern GPIO led;
//extern GPIO swdio, swclk;
//extern TIM tim1;
//extern USART usart1;
//extern bxCAN can1;

class IO_Board
{
public:
    IO_Board();
    ~IO_Board();
    void peripheralSetup(void);
    void interfaceSetup(void);

    void cycleTransmit(void);

private:
    void RCC_Setup(void);
    void GPIO_Setup(void);
    void TIM_Setup(void);
    void USART_Setup(void);
    void bxCAN_Setup(void);
    void IWDG_Setup(void);
};
