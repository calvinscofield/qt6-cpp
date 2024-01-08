#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event); // 拖动文件进入窗口时触发的事件处理函数
    void dragEnterEvent(QDragEnterEvent *event); // 窗口改变大小时触发的事件处理函数
    void dropEvent(QDropEvent *event); // 拖动文件在窗口上放置时触发的事件处理函数
};
#endif // WIDGET_H
