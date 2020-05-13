#include "serialport.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "../interrupt.hpp"

SerialPort port1;

SerialPort::SerialPort()
{

}

SerialPort::SerialPort(USART* usart)
{
    port = usart;
}

void SerialPort::setup(USART* usart)
{
    port = usart;
}

SerialPort::~SerialPort()
{

}

//! バッファクリア
void SerialPort::clear(void)
{
    tx.clear();
    rx.clear();
}

//int SerialPort::putchar_(uint8_t c)
//{
//    if(!(tx.enqueue(c))) return EOF;
//    return (int)c;
//}
//
//int SerialPort::getchar_(void)
//{
//    uint8_t c;
//    if(!(rx.dequeue(&c))) return EOF;
//    return (int)c;
//}

int SerialPort::write(const uint8_t* data, int size)
{
    const uint8_t* p = data;
    int cnt = 0;

    if(p == NULL) return 0;
    while(size > 0)
    {
        if(!(tx.enqueue(*(p++)))) break;
        cnt++;
        size--;
    }
    return cnt;
}

int SerialPort::read(uint8_t* data, int size)
{
    uint8_t* p = data;
    int cnt = 0;

    if(p == NULL) return 0;
    while(size > 0)
    {
        if(!(rx.dequeue(p++))) break;
        cnt++;
        size--;
    }
    return cnt;
}

int SerialPort::printf(const char* format, ...)
{
    char buffer[128];
    va_list ap;
    int len;
    va_start(ap, format);
    len = vsprintf(buffer, format, ap);
    va_end(ap);
    return write((uint8_t*)buffer, len);
}

ByteQueue::ByteQueue()
{
    clear();
}

bool ByteQueue::enqueue(uint8_t data)
{
    ProtectInterrupt pi;
    //bool intLock = isInterruptUnlocked();
    int pNext;

    //if(intLock) lockInterrupt();
    pNext = (tail + 1) & qPointerMask;
    if(pNext == front)
    {
        //一杯
        //if(intLock) unlockInterrupt();
        return false;
    }
    qData[tail] = data;
    tail = pNext;
    storedSize++;
    //if(intLock) unlockInterrupt();
    return true;
}

bool ByteQueue::dequeue(uint8_t* data)
{
    ProtectInterrupt pi;
    //bool intLock = isInterruptUnlocked();

    //if(intLock) lockInterrupt();
    if(front == tail)
    {
        //空
        //if(intLock) unlockInterrupt();
        return false;
    }
    *data = qData[front];
    front = (front + 1) & qPointerMask;
    storedSize--;
    //if(intLock) unlockInterrupt();
    return true;
}

int ByteQueue::getStoredSize(void)
{
    return storedSize;
}

bool ByteQueue::isEmpty(void)
{
    if(storedSize == 0) return true;
    else return false;
}

bool ByteQueue::isFull(void)
{
    if(storedSize == qPointerMask) return true;
    else return false;
}

void ByteQueue::clear(void)
{
    ProtectInterrupt pi;
    //bool intLock = isInterruptUnlocked();

    //if(intLock) lockInterrupt();
    tail = 0;
    front = 0;
    storedSize = 0;
    //if(intLock) unlockInterrupt();
}

bool ByteQueue::peak(uint8_t* data)
{
    if(storedSize != 0)
    {
        *data = qData[front];
        return true;
    }
    return false;
}
