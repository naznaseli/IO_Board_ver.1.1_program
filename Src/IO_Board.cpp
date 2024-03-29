#include "IO_Board.hpp"
//#include <machine.h>

//! GPIOピン
GPIO io[IO_NUM];
////! LED
GPIO led;
GPIO dip[4];
GPIO swdio, swclk;
TIM tim3;
USART usart1;
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
    io[0].setup(PB10, GPIO::FLOATING);
    io[1].setup(PB2, GPIO::FLOATING);
    io[2].setup(PB1, GPIO::FLOATING);
    io[3].setup(PB0, GPIO::FLOATING);
    io[4].setup(PA7, GPIO::FLOATING);
    io[5].setup(PA6, GPIO::FLOATING);
    io[6].setup(PA5, GPIO::FLOATING);
    io[7].setup(PA4, GPIO::FLOATING);
    io[8].setup(PB9, GPIO::FLOATING);
    io[9].setup(PB8, GPIO::FLOATING);
    io[10].setup(PB7, GPIO::FLOATING);
    io[11].setup(PB6, GPIO::FLOATING);
    io[12].setup(PB5, GPIO::FLOATING);
    io[13].setup(PB4, GPIO::FLOATING);
    io[14].setup(PB3, GPIO::FLOATING);
    io[15].setup(PA15, GPIO::FLOATING);

    //LED
    led.setup(PB11, GPIO::PUSHPULL);

    //DIPスイッチ
    dip[0].setup(PA3, GPIO::FLOATING);
    dip[1].setup(PA2, GPIO::FLOATING);
    dip[2].setup(PA1, GPIO::FLOATING);
    dip[3].setup(PA0, GPIO::FLOATING);

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
    usart1.setup(USART1, PA9, PA10, 115200);
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
}

void IO_Board::interrupt(void)
{
    m_elapsedTime++;
    if(m_delayCnt) m_delayCnt--;
    if(m_buzzerCnt) m_buzzerCnt--;

    static uint16_t cnt = 0;
    if (++cnt >= 100)
    {
        cnt = 0;
        led.toggle();
    }
    
    //センサ読み取り
    uint16_t ioVal = 0;
    for(int i = 0; i < IO_NUM; i++)
    {
        ioVal += ((io[i].read() & 0x01) << i);
    }

    //debug print
    static uint16_t cnt2 = 0;
    if (++cnt2 >= 100)
    {
        cnt2 = 0;
        for(int i = 0; i < IO_NUM; i++)
        {
            usart1.printf("%u", (ioVal >> i) & 0x01);
        }
        usart1.printf("\n");
    }

    //CAN送信

}
