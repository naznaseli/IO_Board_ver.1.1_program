#include "serialport.hpp"

SerialPort::SerialPort(USART* usart)
{
    port = usart;
}
