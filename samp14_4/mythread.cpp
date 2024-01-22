#include "mythread.h"

#include <QSemaphore>

#define BUF_SIZE 10 // 缓冲区数据点个数
int buffer1[BUF_SIZE];
int buffer2[BUF_SIZE];
int curBufNum = 1; // 当前正在写入的缓冲区编号
int bufSeq = 0; // 缓冲区序号
QSemaphore emptyBufs(2); // 信号量，空的缓冲区个数，初始资源个数为2
QSemaphore fullBufs; // 信号量，满的缓冲区个数，初始资源个数为0

void TDaqThread::run()
{
    curBufNum = 1; // 当前正在写入的缓冲区编号
    bufSeq = 0; // 缓冲区序号
    int counter = 0; // 模拟数据
    int n = emptyBufs.available();
    if (n < 2) // 确保线程启动时emptyBufs.available() == 2
        emptyBufs.release(2 - n);
    m_stop = false;
    while (!m_stop)
    {
        emptyBufs.acquire(); // 获取一个空的缓冲区
        int *buf = curBufNum == 1 ? buffer1 : buffer2; // 设置当前工作缓冲区指针
        for (int i = 0; i < BUF_SIZE; i++) // 产生一个缓冲区的数据
        {
            *buf = counter;
            buf++;
            counter++;
            msleep(10);
        }
        bufSeq++; // 缓冲区序号
        curBufNum = curBufNum == 1 ? 2 : 1; // 切换当前写入的缓冲区编号
        fullBufs.release(); // fullBufs释放一个资源，有了一个满的缓冲区
    }
}

TDaqThread::TDaqThread(QObject *parent) : QThread(parent)
{

}

void TDaqThread::stopThread()
{
    m_stop = true;
}

void TProcessThread::run()
{
    int n = fullBufs.available();
    if (n > 0)
        fullBufs.acquire(n); // 将fullBufs可用资源个数初始话为0
    int bufData[BUF_SIZE];
    m_stop = false;
    while (!m_stop)
    {
        if (fullBufs.tryAcquire(1, 50)) // 尝试获取一个资源，最多等待50ms
        {
            int *bufferFull = curBufNum == 1 ? buffer2 : buffer1; // 获取已写满的缓冲区的指针
            for (int i = 0; i < BUF_SIZE; i++) // 模拟数据处理
                bufData[i] = *bufferFull;
            emptyBufs.release(); // emptyBufs释放一个资源，可用空缓冲区个数加1
            int pointCount = BUF_SIZE;
            emit dataAvailable(bufSeq, bufData, pointCount); // 发射信号
        }
    }
}

TProcessThread::TProcessThread(QObject *parent) : QThread(parent)
{

}

void TProcessThread::stopThread()
{
    m_stop = true;
}
