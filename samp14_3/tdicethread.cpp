#include "tdicethread.h"

#include <QRandomGenerator>
#include <QReadWriteLock>
#include <QWaitCondition>

// 文件内的全局变量
QReadWriteLock rwLocker; // 读写锁
QWaitCondition waiter; // 等待条件
int seq = 0, diceValue = 0;

TDiceThread::TDiceThread(QObject *parent)
    : QThread(parent)
{

}

void TDiceThread::run()
{
    seq = 0;
    while (1)
    {
        rwLocker.lockForWrite(); // 以写入方式锁定
        diceValue = QRandomGenerator::global()->bounded(1, 7);
        seq++;
        rwLocker.unlock(); // 解锁
        waiter.wakeAll(); // 唤醒其他等待的线程
        msleep(500);
    }
    // 在m_stop == true时结束线程任务
    quit(); // 相当于exit(0)，退出线程的事件循环
}


TValueThread::TValueThread(QObject *parent) : QThread(parent)
{

}

void TValueThread::run()
{
    while (1)
    {
        rwLocker.lockForRead(); // 以只读方式锁定
        waiter.wait(&rwLocker); // 等待被唤醒
        emit newValue(seq, diceValue);
        rwLocker.unlock(); // 解锁
    }
}

TPictureThread::TPictureThread(QObject *parent) : QThread(parent)
{

}

void TPictureThread::run()
{
    while (1)
    {
        rwLocker.lockForRead(); // 以只读方式锁定
        waiter.wait(&rwLocker); // 等待被唤醒
        QString filename = QString::asprintf(":/dice/images/d%d.jpg", diceValue);
        emit newPicture(filename);
        rwLocker.unlock(); // 解锁
    }
}
