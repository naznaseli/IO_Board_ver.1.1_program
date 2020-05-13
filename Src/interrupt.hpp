#pragma once

#include "stm32f103xb.h"

//! 割り込み禁止
class ProtectInterrupt
{
public:
    ProtectInterrupt()
    {
        __disable_irq();
    };

    ~ProtectInterrupt()
    {
        __enable_irq();
    };
};
