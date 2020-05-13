/*
 * DMA.hpp
 *
 *  Created on: 2019/05/09
 *      Author: Shibata
 */

#ifndef DMA_HPP_
#define DMA_HPP_

#include "stm32f103xb.h"

#ifdef __cplusplus
extern "C"{
#endif
//USART2_RX
void DMA1_Channel6_IRQHandler(void);
//USART2_TX
void DMA1_Channel7_IRQHandler(void);
#ifdef __cplusplus
}
#endif

class DMA{
public:
    void setup(DMA_TypeDef *dma, int ch);
    DMA_TypeDef *DMAx;
    int ch;

private:
};

//DMA dma_usart2tx;
//dma_usart2_tx.setup(DMA1, 7);
//usart2.setup(USART2, usart2_tx, usart2_rx, 115200, dma_usart2_tx, dma_usart2_rx);

#endif /* DMA_HPP_ */
