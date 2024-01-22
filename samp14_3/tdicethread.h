#ifndef TDICETHREAD_H
#define TDICETHREAD_H

#include <QThread>

// TDiceThread 是生成骰子点数的线程
class TDiceThread : public QThread
{
    Q_OBJECT
public:
    explicit TDiceThread(QObject *parent = nullptr);

    // QThread interface
protected:
    void run(); // 线程的任务函数
};

// TValueThread 获取骰子点数
class TValueThread : public QThread
{
    Q_OBJECT
public:
    explicit TValueThread(QObject *parent = nullptr);
signals:
    void newValue(int seq, int diceValue);

    // QThread interface
protected:
    void run();
};

// TPictureThread 获取骰子点数，生成对应的图片文件名
class TPictureThread : public QThread
{
    Q_OBJECT
public:
    explicit TPictureThread(QObject *parent = nullptr);
signals:
    void newPicture(QString &picName);

    // QThread interface
protected:
    void run();
};

#endif // TDICETHREAD_H
