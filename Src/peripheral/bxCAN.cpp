#include "bxCAN.hpp"
#include "../IO_Board.hpp"
#include <stdint.h>

#define writeBit(PERIPH, REG, SYM, VAL) (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)      (PERIPH->REG)
#define readBit(PERIPH, REG, SYM)       (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg(PERIPH, REG)  ()

void USB_HP_CAN_TX_IRQHandler(void);
void USB_LP_CAN_RX0_IRQHandler(void);
void CAN_RX1_IRQHandler(void);

void USB_HP_CAN_TX_IRQHandler(void)
{
    usart1.printf("tx.\n");
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
    //usart1.printf("received0.\n");
    //CAN1->RF1R |= CAN_RF1R_RFOM1;
    if (CAN1->RF0R & CAN_RF1R_FMP1)
    {
        usart1.printf("FMP");
        usart1.printf("%u\n", (uint32_t)(CAN1->RF0R & 0x03)); //FMP0

        //読み取り
        //SID表示
        usart1.printf("SID:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RIR >> 21 & 0x7FF));
        usart1.printf("length:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RDTR & 0xF));
        //usart1.printf("data:%u\n", (uint32_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFFFF));

        uint32_t test[2] = {0};
        test[0] = (uint32_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFF);
        test[1] = (uint32_t)((CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF);

        usart1.printf("data0:");
        for (int i = 0; i < 8; i++)
        {
            usart1.printf("%u", (test[0] >> i) & 0x01);
        }
        usart1.printf("\n");
        usart1.printf("data0:");
        for (int i = 0; i < 8; i++)
        {
            usart1.printf("%u", (test[1] >> i) & 0x01);
        }
        usart1.printf("\n");

        //
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
    if(CANx == CAN1){
        NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
        NVIC_EnableIRQ(CAN1_RX1_IRQn);
    }

    //割り込み許可
    CANx->IER |= (CAN_IER_FOVIE0 | CAN_IER_FFIE0 | CAN_IER_FMPIE0);
    CANx->IER |= CAN_IER_ERRIE;

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
    writeBit(CAN1, BTR, BRP, 5);    //6Tq-1
    writeBit(CAN1, BTR, TS1, 6);    //7Tq-1
    writeBit(CAN1, BTR, TS2, 3);    //4Tq-1
    writeBit(CAN1, BTR, SJW, 0);    //1Tq-1
    //writeBit(CAN1, BTR, BRP, 1-1);    //1-1
    //writeBit(CAN1, BTR, TS1, 4-1);    //4-1
    //writeBit(CAN1, BTR, TS2, 3-1);    //3-1
    //writeBit(CAN1, BTR, SJW, 1-1);    //1-1
    //CAN->MCR
    //自動再送信禁止
    //CAN1->MCR |= CAN_MCR_NART;

    //モード
    //CAN1->BTR |= CAN_BTR_LBKM;  //ループバックモード
    //CAN1->BTR |= CAN_BTR_SILM;  //サイレントモード
    //writeBit(CAN1, BTR, LBKM);
    //writeBit(CAN1, BTR, SILM);

    //フィルタ
    CAN1->FMR |= CAN_FMR_FINIT;

    //フィルタバンクの構成
    //IDリストモード
    CAN1->FM1R |= CAN_FM1R_FBM0;

    //16bit x 2
    CAN1->FS1R &= ~(CAN_FS1R_FSC0);

    //フィルタをFIFO0に設定
    CAN1->FFA1R &= ~(CAN_FFA1R_FFA0);

    CAN1->sFilterRegister[0].FR1 = 512 << 5;

    CAN1->FA1R |= CAN_FA1R_FACT0;     //フィルタをアクティブに

    //フィルタ設定完了
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
    //return NONE;
    //なんか返せ
    //TODO
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

    default:
        break;
    }
}

//only STDID
void bxCAN::send(uint16_t id, uint8_t length, uint8_t data[8])
{
    //初期化されていなかったら
    //return;
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
    }
    else if(CANx->TSR & CAN_TSR_TME1)
    {
        transmitMailbox = 1;
    }
    else if(CANx->TSR & CAN_TSR_TME2)
    {
        transmitMailbox = 2;
    }

    CANx->sTxMailBox[transmitMailbox].TIR = 0;
    CANx->sTxMailBox[transmitMailbox].TIR |=
                            ((txMessage->StdId << CAN_TI0R_STID_Pos) & CAN_TI0R_STID_Msk) |
                            ((txMessage->ExtId << CAN_TI0R_EXID_Pos) & CAN_TI0R_EXID_Msk) |
                            ((txMessage->IDE << CAN_TI0R_IDE_Pos) & CAN_TI0R_IDE_Msk) |
                            ((txMessage->RTR << CAN_TI0R_RTR_Pos) & CAN_TI0R_RTR_Msk);
    //if(txMessage->IDE == 0)
    //{   //STDID
    //    CANx->sTxMailBox[transmitMailbox].TIR |=
    //                    ((txMessage->StdId << CAN_TI0R_STID_Pos) |
    //                    (txMessage->RTR << CAN_TI0R_RTR_Pos));
    //}
    //else
    //{   //EXTID
    //    CANx->sTxMailBox[transmitMailbox].TIR |=
    //                    ((txMessage->ExtId << CAN_TI0R_STID_Pos) |
    //                    (txMessage->IDE << CAN_TI0R_IDE_Pos) |
    //                    (txMessage->RTR << CAN_TI0R_RTR_Pos));
    //}

    //DLC
    CANx->sTxMailBox[transmitMailbox].TDTR = 0;
    CANx->sTxMailBox[transmitMailbox].TDTR |= (txMessage->DLC & 0xF);

    //データ
    CANx->sTxMailBox[transmitMailbox].TDHR =
            ((uint32_t)txMessage->Data[7] << CAN_TDH0R_DATA7_Pos) & CAN_TDH0R_DATA7_Msk |
            ((uint32_t)txMessage->Data[6] << CAN_TDH0R_DATA6_Pos) & CAN_TDH0R_DATA6_Msk |
            ((uint32_t)txMessage->Data[5] << CAN_TDH0R_DATA5_Pos) & CAN_TDH0R_DATA5_Msk |
            ((uint32_t)txMessage->Data[4] << CAN_TDH0R_DATA4_Pos) & CAN_TDH0R_DATA4_Msk;

    CANx->sTxMailBox[transmitMailbox].TDLR =
            (((uint32_t)txMessage->Data[3] << CAN_TDL0R_DATA3_Pos) & CAN_TDL0R_DATA3_Msk) |
            (((uint32_t)txMessage->Data[2] << CAN_TDL0R_DATA2_Pos) & CAN_TDL0R_DATA2_Msk) |
            (((uint32_t)txMessage->Data[1] << CAN_TDL0R_DATA1_Pos) & CAN_TDL0R_DATA1_Msk) |
            (((uint32_t)txMessage->Data[0] << CAN_TDL0R_DATA0_Pos) & CAN_TDL0R_DATA0_Msk);

    //送信リクエスト
    CANx->sTxMailBox[transmitMailbox].TIR |= CAN_TI0R_TXRQ;
}

//void bxCAN::recv()
//{
//    if(CANx == CAN1) can1_recv();
//
//}
