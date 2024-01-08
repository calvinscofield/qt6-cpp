#ifndef TMYLABEL_H
#define TMYLABEL_H

#include <QLabel>
#include <QWidget>

class TMyLabel : public QLabel
{
    Q_OBJECT
public:
    TMyLabel(QWidget *parent = nullptr); // 构造函数需要按此参数改写
    bool event(QEvent *e); // 重新实现event()函数

protected:
    void mouseDoubleClickEvent(QMouseEvent *event); // 重新实现鼠标双击事件的默认处理函数
signals:
    void doubleClicked(); // 自定义信号
};

#endif // TMYLABEL_H
