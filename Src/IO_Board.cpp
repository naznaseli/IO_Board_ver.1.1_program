#include "IO_Board.hpp"

////! GPIOピン
//GPIO io[16];
////! LED
//GPIO led;
//GPIO swdio, swclk;
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

void IO_Board::peripheralSetup(void)
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

    //SWDピン設定
    //swdio.setup(PA13, GPIO::ALT_PUSHPULL);
    //swclk.setup(PA14, GPIO::ALT_PUSHPULL);
    //setAlternate_JTAG_SWD(JTAG_SWD_MODE::SWDP);
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
