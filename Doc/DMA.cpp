/*
 * DMA.cpp
 *
 *  Created on: 2019/05/09
 *      Author: Shibata
 */

#include "stm32f103xb.h"
#include "DMA.hpp"
#include "SerialPort.hpp"
//#include "Hardware.hpp"
#include "STM32F103C8T6_Board.hpp"

void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

//usart2 rx
void DMA1_Channel6_IRQHandler(void){
    if(DMA1->ISR & DMA_ISR_TCIF6){
        DMA1->IFCR |= DMA_IFCR_CTCIF6;
    }
}

//usart2 tx
void DMA1_Channel7_IRQHandler(void){
    //static int cnt=0;
    //if(++cnt >= 100){
    //    cnt = 0;
    //    led2.toggle();
    //}

    if(DMA1->ISR & DMA_ISR_TCIF7){
        DMA1->IFCR |= DMA_IFCR_CTCIF7;
        DMA1_Channel7->CCR &= ~(DMA_CCR_EN);
    }
}

void DMA::setup(DMA_TypeDef *dma, int ch){
    this->DMAx = dma;
    this->ch = ch;
}
