#include "IO_Board.hpp"

IO_Board* board;

int main(void)
{
    board = new IO_Board();
    board->setupPeripheral();

    usart1.printf("peripheral initialized\n");

    //DIPスイッチ読み取り
    uint8_t dipSw = 15 - ((dip[0].read()) | (dip[1].read() << 1) | (dip[2].read() << 2) | (dip[3].read() << 3));
    board->canSid = BASE_ADDRESS_IO + dipSw;
    usart1.printf("can sid = %u\n", board->canSid);

    //割り込み開始->上のセットアップ関数内でもいいかも
    tim3.enableCount();

    led.write(1);
    //for(int i = 8; i < 16; i++)
    //    io[i].write(1);

    while(1)
    {
        board->cycle();
    }
}

void interrupt(void)
{
    board->interrupt();
}
