#ifndef TMYLABEL_H
#define TMYLABEL_H

#include <QLabel>

class TMyLabel : public QLabel
{
    Q_OBJECT
public:
    TMyLabel(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event); // 鼠标事件处理函数
signals:
    void clicked(); // 自定义信号
};

#endif // TMYLABEL_H
