#include "bxCAN.hpp"
#include <stdio.h>

#define writeBit(PERIPH, REG, SYM, VAL) (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)      (PERIPH->REG)
#define readBit(PERIPH, REG, SYM)       (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg(PERIPH, REG)  ()

void USB_HP_CAN_TX_IRQHandler(void)
{
    printf("tx.\n");

}
void USB_LP_CAN_RX0_IRQHandler(void)
{
    printf("received0.\n");

}
void CAN_RX1_IRQHandler(void)
{
    printf("received1.\n");
}

bxCAN::bxCAN(
        CAN_TypeDef* can, Mode mode,
        GPIO_TypeDef* gpioRx, uint8_t pinRx,
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        uint16_t baudrate)
{
    setup(can, mode, gpioRx, pinRx, gpioTx, pinTx, baudrate);
}

void bxCAN::setup(
        CAN_TypeDef* can, Mode mode,
        GPIO_TypeDef* gpioRx, uint8_t pinRx,
        GPIO_TypeDef* gpioTx, uint8_t pinTx,
        uint16_t baudrate)
{
    CANx = can;
    m_mode = mode;

    rx = new GPIO(gpioRx, pinRx, GPIO::INPUT_PU);
    tx = new GPIO(gpioTx, pinTx, GPIO::ALT_PUSHPULL);
    remap(gpioRx, pinRx, gpioTx, pinTx);

    enableClock();
    //if(CANx == CAN1){
    //    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    //    NVIC_EnableIRQ(CAN1_RX1_IRQn);
    //}
//  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
//  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
//  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */

    //割り込み許可
    //CANx->IER |= (CAN_IER_FOVIE0 | CAN_IER_FFIE0 | CAN_IER_FMPIE0);
    //CANx->IER |= CAN_IER_ERRIE;

    if(CANx == CAN1) can1_setup(baudrate);

}

void bxCAN::enableClock(void)
{
    if(CANx == CAN1)
        writeBit(RCC, APB1ENR, CAN1EN, 1);
}

void bxCAN::disableClock(void)
{
    if(CANx == CAN1)
        writeBit(RCC, APB1ENR, CAN1EN, 0);
}

void bxCAN::remap(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx)
{
    if(gpioRx == GPIOA && pinRx == 11 && gpioTx == GPIOA && pinTx == 12)
        writeBit(AFIO, MAPR, CAN_REMAP, 0);
    if(gpioRx == GPIOB && pinRx == 8 && gpioTx == GPIOB && pinTx == 9)
        writeBit(AFIO, MAPR, CAN_REMAP, 2);
    if(gpioRx == GPIOD && pinRx == 0 && gpioTx == GPIOD && pinTx == 1)
        writeBit(AFIO, MAPR, CAN_REMAP, 3);
}

void bxCAN::can1_setup(uint16_t baudrate)
{
    //初期化モード
    modeTransition(INITIALIZATION);

    //INRQセット
    //SLEEPビット同時にクリア

    //ボーレート等設定
    //baurate = 1/(500*1000)=12Tq=(BRP+1)*1/(36*1000000)より
    //BRP+1 = 6
    //CAN1->BTR &= ~(CAN_BTR_BRP | CAN_BTR_TS1 | CAN_BTR_TS2 | CAN_BTR_SJW);
    //writeBit(CAN1, BTR, BRP, 6-1);
    //writeBit(CAN1, BTR, TS1, 7-1);
    //writeBit(CAN1, BTR, TS2, 4-1);
    //writeBit(CAN1, BTR, SJW, 1-1);
    writeBit(CAN1, BTR, BRP, 1-1);
    writeBit(CAN1, BTR, TS1, 4-1);
    writeBit(CAN1, BTR, TS2, 3-1);
    writeBit(CAN1, BTR, SJW, 1-1);
    //CAN->MCR
    //自動再送信禁止
    //CAN1->MCR |= CAN_MCR_NART;

    //モード
    CAN1->BTR |= CAN_BTR_LBKM;  //ループバックモード
    //CAN1->BTR |= CAN_BTR_SILM;  //サイレントモード
    //writeBit(CAN1, BTR, LBKM);
    //writeBit(CAN1, BTR, SILM);

    //フィルタ
    //フィルタ設定しないと受信できない可能性
    CAN1->FMR &= ~(CAN_FMR_FINIT);

    //通常モード移行
    modeTransition(NORMAL);
}

bxCAN::Mode bxCAN::modeNow(void)
{
    if(!(CANx->MSR & CAN_MSR_INAK) && !(CANx->MSR & CAN_MSR_INAK))
        return NORMAL;
    if(CANx->MSR & CAN_MSR_SLAK)
    //if(read(CAN1, MSR, SLAK, 1))
        return SLEEP;
    if(CANx->MSR & CAN_MSR_INAK)
    //if(read(CAN1, MSR, INAK, 1))
        return INITIALIZATION;

    if((CANx->BTR & CAN_BTR_SILM) && (CANx->BTR & CAN_BTR_LBKM))
    //if(read(CAN1, BTR, SILM, ) && read(CAN1, BTR, LBKM, 1))
        return SILENT_LOOPBACK;
    if(CANx->BTR & CAN_BTR_SILM)
        return SILENT;
    if(CANx->BTR & CAN_BTR_LBKM)
        return LOOPBACK;

}

void bxCAN::modeTransition(Mode mode)
{
    switch(mode)
    {
        case SLEEP:
            break;

        case INITIALIZATION:
            if(CANx->MSR & CAN_MSR_SLAK)    //if sleep mode now
                CANx->MCR &= ~(CAN_MCR_SLEEP);
            CANx->MCR |= CAN_MCR_INRQ;
            while(((CANx->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) || ((CANx->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK));
            //if(CANx == CAN1){
            //    writeBit(CAN1, MCR, SLEEP, 0);
            //    writeBit(CAN1, MCR, INRQ, 1);
            //    while(read(CAN1, MSR, INAK) || read(CAN1, MSR, SLAK));
            //}
            break;

        case NORMAL:
            if(CANx->MSR & CAN_MSR_SLAK)    //if sleep mode now
                CANx->MCR &= ~(CAN_MCR_SLEEP);
            CANx->MCR &= ~(CAN_MCR_INRQ);
            while(((CANx->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) || ((CANx->MSR & CAN_MSR_SLAK) == CAN_MSR_SLAK));
            break;

        default: break;
    }
}

//only STDID
void bxCAN::send(uint16_t id, uint8_t length, uint8_t data[8])
{
    //メッセージ作成
    CanMsg txMessage;
    txMessage.StdId = 0;
    txMessage.ExtId = 0;
    txMessage.IDE = 0;
    txMessage.RTR = 0;
    txMessage.DLC = 0;
    for(int i = 0; i < 8; i++)
        txMessage.Data[i] = 0;

    txMessage.StdId = id;
    txMessage.DLC = length;
    for(int i = 0; i < length; i++)
        txMessage.Data[i] = data[i];

    can_transmit(&txMessage);
}

void bxCAN::can_transmit(CanMsg* txMessage)
{
    //initializedでない、sleepのとき
    //return;
    //すべてのメールボックスが空いていない
    //if() return;

    //EMPTY状態のメールボックスの番号を代入
    //uint8_t transmitMailbox = (CANx->TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
    uint8_t transmitMailbox = 0;
    if(CANx->TSR & CAN_TSR_TME0)
    {
        transmitMailbox = 0;
    }else if(CANx->TSR & CAN_TSR_TME1)
    {
        transmitMailbox = 1;
    }else if(CANx->TSR & CAN_TSR_TME2)
    {
        transmitMailbox = 2;
    }

    //標準
    if(txMessage->IDE == 0) //標準ID
    {
        //STDID
        CANx->sTxMailBox[transmitMailbox].TIR |=
                        ((txMessage->StdId << CAN_TI0R_STID_Pos) |
                        (txMessage->RTR << CAN_TI0R_RTR_Pos));
    }else
    {
        //EXTID
        CANx->sTxMailBox[transmitMailbox].TIR |=
                        ((txMessage->ExtId << CAN_TI0R_STID_Pos) |
                        (txMessage->IDE << CAN_TI0R_IDE_Pos) |
                        (txMessage->RTR << CAN_TI0R_RTR_Pos));
    }

    //DLC
    CANx->sTxMailBox[transmitMailbox].TDTR = (txMessage->DLC);

    //データ
    CANx->sTxMailBox[transmitMailbox].TDHR =
            ((uint32_t)txMessage->Data[3] << CAN_TDH0R_DATA7_Pos) |
            ((uint32_t)txMessage->Data[2] << CAN_TDH0R_DATA6_Pos) |
            ((uint32_t)txMessage->Data[1] << CAN_TDH0R_DATA5_Pos) |
            ((uint32_t)txMessage->Data[0] << CAN_TDH0R_DATA4_Pos);
    CANx->sTxMailBox[transmitMailbox].TDLR =
            ((uint32_t)txMessage->Data[3] << CAN_TDL0R_DATA3_Pos) |
            ((uint32_t)txMessage->Data[2] << CAN_TDL0R_DATA2_Pos) |
            ((uint32_t)txMessage->Data[1] << CAN_TDL0R_DATA1_Pos) |
            ((uint32_t)txMessage->Data[0] << CAN_TDL0R_DATA0_Pos);

    //送信リクエスト
    CANx->sTxMailBox[transmitMailbox].TIR |= CAN_TI0R_TXRQ;
}

//void bxCAN::recv()
//{
//    if(CANx == CAN1) can1_recv();
//
//}
