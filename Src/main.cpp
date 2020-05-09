#include "IO_Board.hpp"
#include <stddef.h>

IO_Board* board;

int main(void)
{
    board = new IO_Board();
    board->setupPeripheral();

    //DIPスイッチ読み取り
    uint8_t dipSw = (dip[0].read()) | (dip[1].read() << 1) | (dip[2].read() << 2) | (dip[3].read() << 3);
    board->canSid = BASE_ADDRESS_IO + dipSw;

    //割り込み開始->上のセットアップ関数内でもいいかも
    tim3.enableCount();

    led.write(1);
    for(size_t i = 8; i < 16; i++)
        io[i].write(1);

    while(1)
    {
        for (size_t i = 0; i < 1000000; i++);
        board->cycle();
    }
}

void interrupt(void)
{
    //ProtectInterrupt pi;

    //static uint16_t cnt = 0;
    //if(++cnt >= 100)
    //{
    //    //led.toggle();
    //    cnt = 0;
    //}

    board->interrupt();
}
