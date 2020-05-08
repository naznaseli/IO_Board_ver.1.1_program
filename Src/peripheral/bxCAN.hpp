#pragma once

#include <stdint.h>
#include "stm32f103xb.h"
#include "RCC.hpp"
#include "GPIO.hpp"

#define CAN1_MCR_INRQ_Pos   CAN_MCR_INRQ_Pos
#define CAN1_MCR_INRQ_Msk   CAN_MCR_INRQ_Msk
#define CAN1_MCR_SLEEP_Pos  CAN_MCR_SLEEP_Pos
#define CAN1_MCR_SLEEP_Msk  CAN_MCR_SLEEP_Msk
#define CAN1_MCR_TXFP_Pos   CAN_MCR_SLEEP_Pos
#define CAN1_MCR_TXFP_Msk   CAN_MCR_TXFP_Msk
#define CAN1_MCR_RFLM_Pos   CAN_MCR_RFLM_Pos
#define CAN1_MCR_RFLM_Msk   CAN_MCR_RFLM_Msk
#define CAN1_MCR_NART_Pos   CAN_MCR_NART_Pos
#define CAN1_MCR_NART_Msk   CAN_MCR_NART_Msk
#define CAN1_MCR_AWUM_Pos   CAN_MCR_AWUM_Pos
#define CAN1_MCR_AWUM_Msk   CAN_MCR_AWUM_Msk
#define CAN1_MCR_ABOM_Pos   CAN_MCR_ABOM_Pos
#define CAN1_MCR_ABOM_Msk   CAN_MCR_ABOM_Msk
#define CAN1_MCR_TTCM_Pos   CAN_MCR_TTCM_Pos
#define CAN1_MCR_TTCM_Msk   CAN_MCR_TTCM_Msk
#define CAN1_MCR_RESET_Pos  CAN_MCR_RESET_Pos
#define CAN1_MCR_RESET_Msk  CAN_MCR_RESET_Msk
#define CAN1_MCR_DBF_Pos    CAN_MCR_DBF_Pos
#define CAN1_MCR_DBF_Msk    CAN_MCR_DBF_Msk
#define CAN1_MSR_INAK_Pos   CAN_MSR_INAK_Pos
#define CAN1_MSR_INAK_Msk   CAN_MSR_INAK_Msk
#define CAN1_MSR_SLAK_Pos   CAN_MSR_SLAK_Pos
#define CAN1_MSR_SLAK_Msk   CAN_MSR_SLAK_Msk
#define CAN1_MSR_ERRI_Pos   CAN_MSR_ERRI_Pos
#define CAN1_MSR_ERRI_Msk   CAN_MSR_ERRI_Msk
#define CAN1_MSR_WKUI_Pos   CAN_MSR_WKUI_Pos
#define CAN1_MSR_WKUI_Msk   CAN_MSR_WKUI_Msk
#define CAN1_MSR_SLAKI_Pos  CAN_MSR_SLAKI_Pos
#define CAN1_MSR_SLAKI_Msk  CAN_MSR_SLAKI_Msk
#define CAN1_MSR_TXM_Pos    CAN_MSR_TXM_Pos
#define CAN1_MSR_TXM_Msk    CAN_MSR_TXM_Msk
#define CAN1_MSR_RXM_Pos    CAN_MSR_RXM_Pos
#define CAN1_MSR_RXM_Msk    CAN_MSR_RXM_Msk
#define CAN1_MSR_SAMP_Pos   CAN_MSR_SAMP_Pos
#define CAN1_MSR_SAMP_Msk   CAN_MSR_SAMP_Msk
#define CAN1_MSR_RX_Pos     CAN_MSR_RX_Pos
#define CAN1_MSR_RX_Msk     CAN_MSR_RX_Msk

//
#define CAN1_BTR_BRP_Pos    CAN_BTR_BRP_Pos
#define CAN1_BTR_BRP_Msk    CAN_BTR_BRP_Msk
#define CAN1_BTR_TS1_Pos    CAN_BTR_TS1_Pos
#define CAN1_BTR_TS1_Msk    CAN_BTR_TS1_Msk
#define CAN1_BTR_TS2_Pos    CAN_BTR_TS2_Pos
#define CAN1_BTR_TS2_Msk    CAN_BTR_TS2_Msk
#define CAN1_BTR_SJW_Pos    CAN_BTR_SJW_Pos
#define CAN1_BTR_SJW_Msk    CAN_BTR_SJW_Msk
#define CAN1_BTR_BRP_Pos    CAN_BTR_BRP_Pos
#define CAN1_BTR_BRP_Msk    CAN_BTR_BRP_Msk

#ifdef __cplusplus
extern "C"
{
#endif
void USB_HP_CAN_TX_IRQHandler(void);
void USB_LP_CAN_RX0_IRQHandler(void);
void CAN_RX1_IRQHandler(void);
#ifdef __cplusplus
}
#endif

typedef struct
{
    uint16_t StdId;
    uint32_t ExtId;
    uint8_t IDE;
    uint8_t RTR;
    uint8_t DLC;
    uint8_t Data[8];
}CanMsg;

class bxCAN
{
public:
    enum Mode
    {   //動作モード
        INITIALIZATION, //SLAK=0, INAK=1
        NORMAL,         //SLAK=0, SLAK=0
        SLEEP,          //SLAK=1, SLAK=0
        SILENT,
        LOOPBACK,
        SILENT_LOOPBACK
    };
    bxCAN(){};
    bxCAN(CAN_TypeDef* can, Mode mode, GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx, uint16_t baudrate);
    ~bxCAN(){};
    void setup(
            CAN_TypeDef* can, Mode mode,
            GPIO_TypeDef* gpioRx, uint8_t pinRx,
            GPIO_TypeDef* gpioTx, uint8_t pinTx,
            uint16_t baudrate);

    void send(uint16_t id, uint8_t length, uint8_t data[8]);
    //void recv();
    //void abort();
    
private:
    CAN_TypeDef* CANx;
    Mode m_mode;
    GPIO* rx;
    GPIO* tx;

    void remap(GPIO_TypeDef* gpioRx, uint8_t pinRx, GPIO_TypeDef* gpioTx, uint8_t pinTx);
    void enableClock(void);
    void disableClock(void);

    void modeTransition(Mode mode);
    //今のCANのモードを返す
    Mode modeNow(void);

    void can_transmit(CanMsg* txMessage);

    void can1_setup(uint16_t baudrate);
    void can1_send();
    void can1_recv();
};
