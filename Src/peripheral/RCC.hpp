#pragma once

#include "stm32f103xb.h"

//#define bitWrite(PERIPH, REG, SYM, VAL)   (PERIPH->REG)=((PERIPH->REG)&(~PERIPH##_##REG##_##SYM##_Msk))|(VAL<<PERIPH##_##REG##_##SYM##_Pos)
#define bitReadRCC(PERIPH, REG, SYM)         (((PERIPH->REG)&PERIPH##_##REG##_##SYM##_Msk)>>PERIPH##_##REG##_##SYM##_Pos)

//周波数定義
#define HSE     12      //[MHz]
#define HSI     8       //[MHz]
#define PCLK    5000
#define PLLCLK  (12*bitReadRCC(RCC, CFGR, PLLMULL) //0x7 => 9
#define SYSCLK  (HSE & SW PLLCLK & SW HSI & SW)
#define HCLK    ()
#define PCLK1   36  //AHB分周APB1分周
//TIM2,3,4,5,6,7のクロック周波数
//#define TIMXCLK (PCLK1 / APB1プリスケーラ)
#define TIMXCLK     (PCLK1*2)
#define PCLK2
//TIM1,8のクロック周波数
#define TIMxCLK (PCLK2 / APB2プリスケーラ)
#define ADCCLK  (AHB)
#define HCLK_2  (HCLK/2)
//bitReadでレジスタ呼んだマクロ用意して周波数とか定義できないかな

extern void RCC_Setup_72MHz(void);
extern void RCC_Setup_36MHz(void);
