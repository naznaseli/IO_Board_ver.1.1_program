#include "IO_Board.hpp"

IO_Board* board;

int main(void)
{
    board = new IO_Board();
    board->setupPeripheral();

    usart1.printf("peripheral initialized\n");

    //DIPスイッチ読み取り
    uint8_t dipSw = 15 - ((dip[0].read()) | (dip[1].read() << 1) | (dip[2].read() << 2) | (dip[3].read() << 3));
    board->canId = BASE_ADDRESS_IO + dipSw * 2;
    usart1.printf("can id = %u", board->canId);
    usart1.printf("(0x%x)\n", board->canId);

    //timer start
    tim3.enableCount();

    while(1)
    {
        board->cycle();
    }
}

void interrupt(void)
{
    board->interrupt();
}
