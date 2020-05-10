#include "IWDG.hpp"

void IWDG_Setup(void);

void IWDG_Setup(void)
{
    IWDG->KR = 0x5555;  //アクセス保護解除
    while((IWDG->SR & IWDG_SR_PVU) == IWDG_SR_PVU);
    IWDG->PR = 0x3;     //16分周
    while((IWDG->SR & IWDG_SR_RVU) == IWDG_SR_RVU);
    IWDG->RLR = 0x0FFF; //再ロード値設定

    //カウント開始
    IWDG->KR = 0xCCCC;
}

void IWDG_Reset(void)
{
    IWDG->KR = 0xAAAA;
}
