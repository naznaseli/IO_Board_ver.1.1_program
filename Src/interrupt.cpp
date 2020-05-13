#include "interrupt.hpp"
#include "IO_Board.hpp"

//各種(受信、送信、周期)割り込みの内容をここに記述

//:**********************************************************************
//! TIM
//:**********************************************************************
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

//:**********************************************************************
//! USART
//:**********************************************************************
void USART1_IRQHandler(void)
{
    switch(USART1->SR)
    {
    case USART_SR_TC:       //送信完了(TEI)
        USART1->SR &= ~(USART_SR_TC);
        usart1.disableTxI();
        break;
    case USART_SR_TXE:      //送信データレジスタエンプティ(TXI)
        static uint8_t data;
        port1.tx.dequeue(&data);
        USART1->DR = data;
        if(port1.tx.isEmpty())
        {
            //SCI_LastTX(1);
            usart1.enableTxI();
            USART1->CR1 &= ~(USART_CR1_TXEIE);
        }

        break;
    case USART_SR_RXNE:     //受信データ読み出し可能
        USART1->SR &= ~(USART_SR_RXNE);
        //port1.rx.enqueue(USART1->DR);
        break;
    default:
        USART1->SR = 0;
    }
}

//void USART2_IRQHandler(void)
//{
//    switch(USART2->SR)
//    {
//    case USART_SR_TC:
//        USART2->SR &= ~(USART_SR_TC);
//        break;
//    case USART_SR_TXE:
//
//        break;
//    case USART_SR_RXNE:
//        USART2->SR &= ~(USART_SR_RXNE);
//        //port2.rx.enqueue();
//        break;
//    default:
//        USART2->SR = 0;
//    }
//
//}
//
//void USART3_IRQHandler(void)
//{
//
//}

//:**********************************************************************
//! CAN
//:**********************************************************************
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
        //usart1.printf("SID:%u, length:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RIR >> 21 & 0x7FF), (uint32_t)(CAN1->sFIFOMailBox[0].RDTR & 0xF));
        port1.printf("SID:%u, length:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RIR >> 21 & 0x7FF), (uint32_t)(CAN1->sFIFOMailBox[0].RDTR & 0xF));

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
