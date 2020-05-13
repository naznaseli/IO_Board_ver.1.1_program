#pragma once

#include <stdint.h>
#include "stm32f103xb.h"
#include "RCC.hpp"
#include "GPIO.hpp"

#ifdef __cplusplus
extern "C"
{
#endif
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
#ifdef __cplusplus
}
#endif

class USART
{
public:
    enum Mode
    {
        DEFAULT, QUEUE, DMA
    };
    USART(){};
    ~USART(){};
    
    void setup(
        USART_TypeDef* usart,
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx,
        uint32_t baudrate);
    void setup(
            USART_TypeDef* usart,
            GPIO_TypeDef* gpioTx, uint8_t pinTx, Mode modeTx,
            GPIO_TypeDef* gpioRx, uint8_t pinRx, Mode modeRx,
            uint32_t baudrate);
    void remap(
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx);

    void enableClock(void);
    void disableClock(void);
    void setRemap(void);

    bool isEnable(void);
    void enable(void);
    void disable(void);

    bool isEnableTx(void);
    void enableTx(void);
    void disableTx(void);

    bool isEnableRx(void);
    void enableRx(void);
    void disableRx(void);

    bool isEnableTxI(void);
    void enableTxI(void);
    void disableTxI(void);
    
    bool isEnableRxI(void);
    void enableRxI(void);
    void disableRxI(void);
    void clear(void);

    int putchar_(uint8_t c);
    int getchar_(void);
    int write(const uint8_t* data, int size);
    int read(uint8_t* data, int size);
    int printf(const char* format, ...);

private:
    USART_TypeDef* USARTx;
    Mode m_modeTx;
    Mode m_modeRx;
    
    GPIO* tx;
    GPIO* rx;

    int default_putchar(uint8_t c);
    int default_getchar(void);
    int default_write(const uint8_t* data, int size);
    int default_read(uint8_t* data, int size);
    int default_printf(const char* format, ...);
    int queue_putchar(uint8_t c);
    int queue_getchar(void);
    int queue_write(const uint8_t* data, int size);
    int queue_read(uint8_t* data, int size);
    int queue_printf(const char* format, ...);
    int dma_putchar(uint8_t c);
    int dma_getchar(void);
    int dma_write(const uint8_t* data, int size);
    int dma_read(uint8_t* data, int size);
    int dma_printf(const char* format, ...);
};
