#include "IO_Board.hpp"

IO_Board* board;

int main(void)
{
    board = new IO_Board();
    board->setupPeripheral();

    //DIPスイッチ読み取り
    board->dipSw = (dip[0].read()) | (dip[1].read() << 1) | (dip[2].read() << 2) | (dip[3].read() << 3);

    //割り込み開始->上のセットアップ関数内でもいいかも
    //tim1.enable

    led.write(1);

    while(1)
    {
        board->cycle();
    }
}

void interrupt(void)
{
    //ProtectInterrupt pi;

    static uint16_t cnt = 0;
    if(++cnt >= 100)
    {
        //led.toggle();
        cnt = 0;
    }

    board->interrupt();
}
