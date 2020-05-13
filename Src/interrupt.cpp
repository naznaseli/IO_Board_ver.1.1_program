#include "interrupt.hpp"
#include "IO_Board.hpp"

//各種(受信、送信、周期)割り込みの内容をここに記述

//void TIM1_IRQHandler(void)
//{
//    TIM1->SR = 0;
//}
//
//void TIM2_IRQHandler(void)
//{
//    TIM2->SR = 0;
//}

//1ms interrupt
void TIM3_IRQHandler(void)
{
    TIM3->SR = 0;
    board->interrupt();
}

//void TIM4_IRQHandler(void)
//{
//    TIM4->SR = 0;
//}

void USB_HP_CAN_TX_IRQHandler(void)
{
    usart1.printf("tx.\n");
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
    if (CAN1->RF0R & CAN_RF1R_FMP1)
    {
        //usart1.printf("FMP");
        //usart1.printf("%u\n", (uint32_t)(CAN1->RF0R & 0x03)); //FMP0

        //読み取り
        //SID表示
        //usart1.printf("SID:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RIR >> 21 & 0x7FF));
        port1.printf("SID:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RIR >> 21 & 0x7FF));
        //usart1.printf("length:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RDTR & 0xF));
        port1.printf("length:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RDTR & 0xF));

        uint32_t test[2] = {0};
        test[0] = (uint32_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFF);
        test[1] = (uint32_t)((CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF);

        //usart1.printf("data0:");
        port1.printf("data0:");
        for (int i = 0; i < 8; i++)
        {
            //usart1.printf("%u", (test[0] >> i) & 0x01);
            port1.printf("%u", (test[0] >> i) & 0x01);
        }
        //usart1.printf("\n");
        port1.printf("\n");
        //usart1.printf("data0:");
        port1.printf("data0:");
        for (int i = 0; i < 8; i++)
        {
            //usart1.printf("%u", (test[1] >> i) & 0x01);
            port1.printf("%u", (test[1] >> i) & 0x01);
        }
        //usart1.printf("\n");
        port1.printf("\n");

        CAN1->RF0R |= CAN_RF1R_RFOM1;
    }
    if (CAN1->RF0R & CAN_RF1R_FULL1)
    {
        usart1.printf("FULL\n");
    }
    if (CAN1->RF0R & CAN_RF1R_FOVR1)
    {
        usart1.printf("FOVR\n");
    }
}

void CAN_RX1_IRQHandler(void)
{
    usart1.printf("received1.\n");
}
