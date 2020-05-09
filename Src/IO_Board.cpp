#include "IO_Board.hpp"
//#include <machine.h>

//! GPIOピン
GPIO io[16];
////! LED
GPIO led;
GPIO dip[4];
GPIO swdio, swclk;
TIM tim3;
//USART usart1;
//bxCAN can1;

//__io_getchar(void);
//__io_putchar(int ch);

//int __io_getchar(void)
//{
//    return usart1.getchar();
//}
//
//int __io_putchar(int ch)
//{
//    return usart1.putchar((uint8_t)ch);
//}

IO_Board::IO_Board()
{
    m_delayCnt = 0;
    m_buzzerCnt = 0;
    m_elapsedTime = 0;
}

IO_Board::~IO_Board()
{

}

void IO_Board::setupPeripheral(void)
{
    //クロック設定
    RCC_Setup();

    //I/O設定
    GPIO_Setup();

    //タイマ設定
    TIM_Setup();

    //USART設定
    USART_Setup();

    //CAN設定
    bxCAN_Setup();

    //ウォッチドッグタイマ設定
    IWDG_Setup();

}

void IO_Board::RCC_Setup(void)
{
    RCC_Setup_72MHz();
}

void IO_Board::GPIO_Setup(void)
{
    //IOピン
    io[0].setup(PB10, GPIO::OPENDRAIN);
    io[1].setup(PB2, GPIO::OPENDRAIN);
    io[2].setup(PB1, GPIO::OPENDRAIN);
    io[3].setup(PB0, GPIO::OPENDRAIN);
    io[4].setup(PA7, GPIO::OPENDRAIN);
    io[5].setup(PA6, GPIO::OPENDRAIN);
    io[6].setup(PA5, GPIO::OPENDRAIN);
    io[7].setup(PA4, GPIO::OPENDRAIN);
    //io[8].setup(PB9, GPIO::OPENDRAIN);
    //io[9].setup(PB8, GPIO::OPENDRAIN);
    //io[10].setup(PB7, GPIO::OPENDRAIN);
    //io[11].setup(PB6, GPIO::OPENDRAIN);
    //io[12].setup(PB5, GPIO::OPENDRAIN);
    //io[13].setup(PB4, GPIO::OPENDRAIN);
    //io[14].setup(PB3, GPIO::OPENDRAIN);
    //io[15].setup(PA15, GPIO::OPENDRAIN);

    io[8].setup(PB9, GPIO::PUSHPULL);
    io[9].setup(PB8, GPIO::PUSHPULL);
    io[10].setup(PB7, GPIO::PUSHPULL);
    io[11].setup(PB6, GPIO::PUSHPULL);
    io[12].setup(PB5, GPIO::PUSHPULL);
    io[13].setup(PB4, GPIO::PUSHPULL);
    io[14].setup(PB3, GPIO::PUSHPULL);
    io[15].setup(PA15, GPIO::PUSHPULL);

    //LED
    led.setup(PB11, GPIO::PUSHPULL);

    //DIPスイッチ
    dip[0].setup(PA3, GPIO::OPENDRAIN);
    dip[1].setup(PA2, GPIO::OPENDRAIN);
    dip[2].setup(PA1, GPIO::OPENDRAIN);
    dip[3].setup(PA0, GPIO::OPENDRAIN);

    //SWDピン設定
    swdio.setup(PA13, GPIO::ALT_PUSHPULL);
    swclk.setup(PA14, GPIO::ALT_PUSHPULL);
    setAlternate_JTAG_SWD(JTAG_SWD_MODE::SWDP);
}

void IO_Board::TIM_Setup(void)
{
    //1ms interrupt
    tim3.setupTimer(TIM3, 12, 1000000);
}

void IO_Board::USART_Setup(void)
{
    //usart1.setup(USART1, PB6, PB7, 115200);
}

void IO_Board::bxCAN_Setup(void)
{
    //500kbps
    //can1.setup(CAN1, bxCAN::NORMAL, PA11, PA12, 500);
}

void IO_Board::IWDG_Setup(void)
{

}

void IO_Board::delay_us(uint16_t us)
{
    //nop();

}

void IO_Board::delay_ms(uint16_t ms)
{
    m_delayCnt = ms;
    while(m_delayCnt);
}

size_t IO_Board::millis(void)
{
    return m_elapsedTime;
}

//! 周期動作関数
void IO_Board::cycle(void)
{
    //ウォッチドッグタイマ
    //Lチカ

    led.toggle();
    //for(int i = 8; i < 16; i++)
    //    io[i].toggle();
}

void IO_Board::interrupt(void)
{
    m_elapsedTime++;
    if(m_delayCnt) m_delayCnt--;
    if(m_buzzerCnt) m_buzzerCnt--;

    static uint16_t cnt = 0;
    if (++cnt >= 1000)
    {
        //led.toggle();
        io[8].toggle();
        cnt = 0;
    }
    
    //センサ読み取り
    //uint16_t ioVal;
    //for(int i = 0; i < 16; i++)
    //{
    //    ioVal |= io[i].read();
    //}

    //CAN送信

}
