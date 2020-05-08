#pragma once

#include <stdint.h>
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

extern GPIO io[16];
extern GPIO led;
extern GPIO dip[4];
extern GPIO swdio, swclk;
//extern TIM tim1;
//extern USART usart1;
//extern bxCAN can1;

class IO_Board
{
public:
    IO_Board();
    ~IO_Board();
    void setupPeripheral(void);
    void setupInterface(void);

    void cycleTransmit(void);

    unsigned int m_delayCnt = 0;
    void delay_us(uint16_t us);
    void delay_ms(uint16_t ms);
    unsigned int m_buzzerCnt = 0;
    void buzzer(uint16_t ms);

    unsigned long long int millis(void);

    void cycle(void);
    void interrupt(void);

    uint8_t dipSw;

private:
    void RCC_Setup(void);
    void GPIO_Setup(void);
    void TIM_Setup(void);
    void USART_Setup(void);
    void bxCAN_Setup(void);
    void IWDG_Setup(void);
};

//! 割り込み禁止
class ProtectInterrupt
{
public:
    ProtectInterrupt()
    {
        //__disable_irq();
    };

    ~ProtectInterrupt()
    {
        //__enable_irq();
    };
};