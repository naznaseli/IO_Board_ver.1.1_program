#include "USART.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define writeBit(PERIPH, REG, SYM, VAL) (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)      (PERIPH->REG)
#define readBit(PERIPH, REG, SYM)       (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg(PERIPH, REG)  ()

void USART::setup(
        USART_TypeDef* usart,
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx,
        uint32_t baudrate)
{
    setup(usart, gpioTx, pinTx, DEFAULT, gpioRx, pinRx, DEFAULT, baudrate);
}

void USART::setup(
        USART_TypeDef* usart,
        GPIO_TypeDef* gpioTx, uint8_t pinTx, Mode modeTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx, Mode modeRx,
        uint32_t baudrate)
{
    USARTx = usart;
    tx = new GPIO(gpioTx, pinTx, GPIO::ALT_PUSHPULL);
    rx = new GPIO(gpioRx, pinRx, GPIO::ALT_PUSHPULL);
    m_modeTx = modeTx;
    m_modeRx = modeRx;

    enableClock();
    
    //ピンのremap確認
    
    //if(m_modeTx == DEFAULT){
    //}else if(m_modeTx == QUEUE){
    //    
    //}else if(m_modeTx == DMA){
    //    
    //}else{
    //    return;
    //}
    //if(m_modeRx == DEFAULT){
    //}else if(m_modeRx == QUEUE){
    //    
    //}else if(m_modeRx == DMA){
    //    
    //}else{
    //    return;
    //}

    //USART1 remap config
    //PB6,PB7
    if(tx->GPIOx == GPIOB && tx->pin == 6 && rx->GPIOx == GPIOB && rx->pin == 7)
    {
        //remap configuration
        //USART1はデフォルトでremapされている(?)
        //2019/05/08　データシート見たけどリセット状態で0って書いてある
        AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
    }else{
        AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP);
    }
    //AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;

    //ここ抽象化
    //ストップビットとかパリティとか設定するprivate関数
    //USARTConfig(stopbit parity, baudrate, );
    //クロック設定と設定ボーレートの値からBRRの値を算出
    if(USARTx == USART1)
    {
        USARTx->BRR = 0x00000271;   //72*100000/115200 = 625 = 0x271
        
        USARTx->CR1 |= USART_CR1_UE;
        //enableUSART();
        USARTx->CR1 |= USART_CR1_TE;
        //enableTx();
        USARTx->CR1 |= USART_CR1_RE;
        //enableRx();
    }
}

void USART::remap(
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        GPIO_TypeDef* gpioRx, uint8_t pinRx)
{
    if(USARTx == USART1)
    {
        if(gpioTx == GPIOA && pinTx == 9 && gpioRx == GPIOA && pinRx == 10)
            writeBit(AFIO, MAPR, USART1_REMAP, 0);
        if(gpioTx == GPIOB && pinTx == 6 && gpioRx == GPIOB && pinRx == 7)
            writeBit(AFIO, MAPR, USART1_REMAP, 1);
    }
    if(USARTx == USART2)
    {
        if(gpioTx == GPIOA && pinTx == 2 && gpioRx == GPIOA && pinRx == 3)
            writeBit(AFIO, MAPR, USART2_REMAP, 0);
        if(gpioTx == GPIOD && pinTx == 5 && gpioRx == GPIOD && pinRx == 6)
            writeBit(AFIO, MAPR, USART2_REMAP, 1);
    }
    if(USARTx == USART3)
    {
        if(gpioTx == GPIOB && pinTx == 10 && gpioRx == GPIOB && pinRx == 11)
            writeBit(AFIO, MAPR, USART3_REMAP, 0);
        if(gpioTx == GPIOC && pinTx == 10 && gpioRx == GPIOC && pinRx == 11)
            writeBit(AFIO, MAPR, USART3_REMAP, 1);
        if(gpioTx == GPIOD && pinTx == 8 && gpioRx == GPIOD && pinRx == 9)
            writeBit(AFIO, MAPR, USART3_REMAP, 3);
    }
}

void USART::enableClock(void)
{
    if(USARTx == USART1) writeBit(RCC, APB2ENR, USART1EN, 1);   //RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    if(USARTx == USART2) writeBit(RCC, APB1ENR, USART2EN, 1);   //RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    if(USARTx == USART3) writeBit(RCC, APB1ENR, USART3EN, 1);   //RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
}

void USART::disableClock(void)
{
    if(USARTx == USART1) writeBit(RCC, APB2ENR, USART1EN, 0);
    if(USARTx == USART2) writeBit(RCC, APB1ENR, USART2EN, 0);
    if(USARTx == USART3) writeBit(RCC, APB1ENR, USART3EN, 0);
}

int USART::putchar_(uint8_t c)
{
    if(m_modeTx == DEFAULT)
    {
        return default_putchar(c);
    }
    else if(m_modeTx == QUEUE)
    {
        //return queue_putchar(c);
    }
    else if(m_modeTx == DMA)
    {
        //return dma_putchar(c);
    }
    return 0;
}

int USART::getchar_(void)
{
    if(m_modeRx == DEFAULT)
    {
        return default_getchar();
    }
    else if(m_modeRx == QUEUE)
    {
        //return queue_getchar();
    }
    else if(m_modeRx == DMA)
    {
        //return dma_getchar();
    }
    return 0;
}

int USART::write(const uint8_t* data, int size)
{
    if(m_modeTx == DEFAULT)
    {
        return default_write(data, size);
    }
    else if(m_modeTx == QUEUE)
    {
        //return queue_write(data, size);
    }
    else if(m_modeTx == DMA)
    {
        //return dma_write(data, size);
    }
    return 0;
}

int USART::read(uint8_t* data, int size)
{
    if(m_modeRx == DEFAULT)
    {
        return default_read(data, size);
    }
    else if(m_modeRx == QUEUE)
    {
        //return queue_read(data, size);
    }
    else if(m_modeRx == DMA)
    {
        //return dma_read(data, size);
    }
    return 0;
}

int USART::printf(const char* format, ...)
{
    if(m_modeTx == DEFAULT)
    {
        return default_printf(format);
    }
    else if(m_modeTx == QUEUE)
    {
        //return queue_printf(format);
    }
    else if(m_modeTx == DMA)
    {
        //return dma_printf(format);
    }
    return 0;
}

//:**********************************************************************
//! default
//:**********************************************************************
int USART::default_putchar(uint8_t c)
{
    while((USARTx->SR & USART_SR_TC) == 0);
    USARTx->DR = c;
    while((USARTx->SR & USART_SR_TXE) == 0);
    
    return (int)c;
}

int USART::default_getchar(void){
    uint8_t c;
    c = USARTx->DR;
    
    return (int)c;
}

int USART::default_write(const uint8_t* data, int size)
{
    const uint8_t* p = data;
    int cnt = 0;
    
    if(p == NULL) return 0;
    while(size > 0)
    {
        default_putchar(*(p++));
        size--;
        cnt++;
    }
    return cnt;
}

int USART::default_read(uint8_t* data, int size)
{
    int cnt = 0;
    while(size > 0)
    {
        default_getchar();
        size--;
        cnt++;
    }
    return cnt;
}

int USART::default_printf(const char* format, ...)
{
    char buffer[128];
    va_list ap;
    int len;
    va_start(ap, format);
    len = vsprintf(buffer, format, ap);
    va_end(ap);
    return default_write((uint8_t*)buffer, len);
}

//:**********************************************************************
//! queue
//:**********************************************************************
//int USART::queue_putchar()
//{
//    
//}

//:**********************************************************************
//! DMA
//:**********************************************************************
