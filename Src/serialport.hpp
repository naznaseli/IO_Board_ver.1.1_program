#pragma once
#include "./peripheral/USART.hpp"

//SerialPortクラス内部でキューの実装
//DMAはまた考える

class SerialPort
{
public:
    SerialPort(USART* usart);
    ~SerialPort();

private:
    USART* port;

};
