#include "IO_Board.hpp"

//! GPIOピン
GPIO io[16];
////! LED
GPIO led;
GPIO dip[4];
GPIO swdio, swclk;
//TIM tim1;
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
    io[8].setup(PB9, GPIO::OPENDRAIN);
    io[9].setup(PB8, GPIO::OPENDRAIN);
    io[10].setup(PB7, GPIO::OPENDRAIN);
    io[11].setup(PB6, GPIO::OPENDRAIN);
    io[12].setup(PB5, GPIO::OPENDRAIN);
    io[13].setup(PB4, GPIO::OPENDRAIN);
    io[14].setup(PB3, GPIO::OPENDRAIN);
    io[15].setup(PA15, GPIO::OPENDRAIN);

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
    //tim1.setupTimer(TIM1, );
}

void IO_Board::USART_Setup(void)
{
    //usart1.setup(USART1, PB6, PB7, 115200);
}

void IO_Board::bxCAN_Setup(void)
{
    //can1.setup(CAN1, bxCAN::NORMAL, PA11, PA12, 500);
}

void IO_Board::IWDG_Setup(void)
{

}

//! 周期動作関数
void IO_Board::cycle(void)
{
    //ウォッチドッグタイマ
    //Lチカ

}

void IO_Board::interrupt(void)
{
    //センサ読み取り
    //uint16_t ioVal;
    //for(int i = 0; i < 16; i++)
    //{
    //    ioVal |= io[i].read();
    //}

    //CAN送信

}
