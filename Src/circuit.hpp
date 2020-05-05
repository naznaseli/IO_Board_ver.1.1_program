#pragma once

#include <stdint.h>

class Circuit
{
public:
Circuit();
~Circuit();

unsigned int m_delayCnt = 0;
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
unsigned int m_buzzerCnt = 0;
void buzzer(uint16_t ms);

//! 起動してから経過時間
//unsigned long long int m_elapsedTime = 0;
unsigned long long int millis(void);

void toggleLed(void);

void setup(void);
void cycle(void);
void interrupt_1ms(void);
void interrupt_10ms(void);

private:
    uint8_t m_dipSw;
    bool m_initialized = false;
    unsigned long long int m_elapsedTime;

    class UserEncoder
    {
    public:
        void flash(uint8_t red, uint8_t green, uint8_t blue);
        void interrupt(void);
        void cycle(void);
        void isPushed(void);

    };
};

//! 割り込み禁止
class ProtectInterrupt
{
public:
    //ProtectInterrupt(){__disable_irq();};
    //~ProtectInterrupt(){__enable_irq();};
};
