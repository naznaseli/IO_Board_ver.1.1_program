#pragma once
//#include "../peripheral/USART.hpp"
#include "../peripheral/USART.hpp"

//SerialPortクラス内部でキューの実装
//DMAはまた考える

class ByteQueue
{
public:
    ByteQueue();

    bool enqueue(uint8_t data);
    bool dequeue(uint8_t* data);
    int getStoredSize(void);
    bool isEmpty(void);
    bool isFull(void);
    void clear(void);
    bool peak(uint8_t* data);

private:
    static const int qSize = 9; //キューサイズ
    static const int qPointerMask = (1 << qSize) - 1;

    uint8_t qData[1 << qSize];
    int tail;
    int front;
    int storedSize;
    bool isInterruptUnlocked(void);
    void lockInterrupt(void);
    void unlockInterrupt(void);

};

class SerialPort
{
public:
    SerialPort();
    SerialPort(USART* usart);
    ~SerialPort();

    ByteQueue tx;
    ByteQueue rx;

    void setup(USART* usart);

    void clear(void);
    int putchar_(uint8_t c);
    int getchar_(void);
    int write(const uint8_t* data, int size);
    int read(uint8_t* data, int size);
    int printf(const char* format, ...);

private:
    USART* port;

};

extern SerialPort port1;
