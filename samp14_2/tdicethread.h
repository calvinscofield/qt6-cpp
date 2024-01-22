#ifndef TDICETHREAD_H
#define TDICETHREAD_H

#include <QMutex>
#include <QThread>

class TDiceThread : public QThread
{
    Q_OBJECT
private:
    QMutex mutex; // 互斥量
    int m_seq = 0; // 掷骰子次数的序号
    int m_diceValue; // 骰子点数
    bool m_paused = true; // 暂停掷骰子
    bool m_stop = false; // 停止线程，也就是退出run()函数的运行
public:
    explicit TDiceThread(QObject *parent = nullptr);
    void diceBegin(); // 开始掷骰子
    void dicePause(); // 暂停
    void stopThread(); // 结束线程
    bool readValue(int *seq, int *diceValue); // 供主线程读取数据的函数

    // QThread interface
protected:
    void run(); // 线程的任务
};

#endif // TDICETHREAD_H
