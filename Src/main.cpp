#include "IO_Board.hpp"

IO_Board* board;
//Controller* controller;

int main(void)
{
    //board = new IO_Board();
    //board.setupPeripheral();

    //回路initialized

    //DIPスイッチ読み取り

    //割り込み開始->上のセットアップ関数内でもいいかも

    while(1)
    {

    }
}

void interrupt_1ms(void)
{
    //ProtectInterrupt pi;

    static uint16_t cnt = 0;
    if(++cnt >= 100)
    {
        //led.toggle();
        cnt = 0;
    }

    //circuit->interrupt_1ms();
}

void interrupt_10ms(void)
{
    //circuit->interrupt_10ms();
    static uint16_t cnt = 0;
    if(++cnt >= 100)
    {
        cnt = 0;
    }

    //センサ読み取り

    //CAN周期送信

}
