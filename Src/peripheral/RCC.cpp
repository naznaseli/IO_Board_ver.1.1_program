#include "RCC.hpp"

//PERIPH->REGにのビットをVALにする
#define writeBit(PERIPH, REG, SYM, VAL) (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define writeReg(PERIPH, REG, VAL)      ()
#define readBit(PERIPH, REG, SYM)       (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)
#define readReg(PERIPH, REG)            ()

void RCC_Setup_72MHz(void);

void RCC_Setup_72MHz(void)
{
    //:**********************************************************************
    //! AHB
    //! FCLK    :72MHz
    //! APB1 Peripheral clock    :36MHz
    //! APB1 Timer clock    :72MHz
    //! APB2 Peripheral clock(PCLK2) :72MHz
    //! APB2 Timer clock    :72MHz
    //! FLIT
    //! (unit:  MHz)
    //:**********************************************************************
    writeBit(RCC, CR, PLLON, 0);
    while(readBit(RCC, CR, PLLRDY) == 1);   //PLL停止待ち

    writeBit(RCC, CR, HSEON, 1);
    while(readBit(RCC, CR, HSERDY) == 0);   //HSE動作待ち
    writeBit(RCC, CFGR, PLLMULL, 0x4);      //PLLx6=72MHz
    //writeBit(RCC, CFGR, PLLMULL, 0x7);      //PLLx9=72MHz
    writeBit(RCC, CFGR, PLLSRC, 1);         //PLLソースにHSE
    writeBit(RCC, CFGR, PPRE1, 0x4);        //2分周
    writeBit(RCC, CFGR, SW, 0x2);           //システムクロックにPLL使用
    writeBit(FLASH, ACR, LATENCY, 4);       //FLASH読み込みの遅延時間設定
    
    writeBit(RCC, CR, PLLON, 1);
    while(readBit(RCC, CR, PLLRDY) == 0);   //PLL安定化待ち

    //:**********************************************************************
    //RCC->CR &= ~RCC_CR_PLLON;
    //while(RCC->CR & RCC_CR_PLLRDY);	//PLL停止待ち

    //RCC->CR |= 0x00010000;  //HSEON
    //while((RCC->CR & RCC_CR_HSERDY) == 0);  //HSE動作待ち
    //RCC->CFGR = 0x001D0402;	//pllx9=72MHz, pll = hse/1, pll選択
    //FLASH->ACR |= FLASH_ACR_LATENCY_2;  //FLASH読み込みの遅延時間設定

    //RCC->CR |= RCC_CR_PLLON;
    //while(!(RCC->CR & RCC_CR_PLLRDY));  //PLL安定化待ち

}
