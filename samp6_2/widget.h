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

private slots:
    void do_doubleClick(); // 与lab的doubleClicked()信号关联

    void on_lab_doubleClicked(); // 也可以自动关联

private:
    Ui::Widget *ui;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event); // 在窗口上双击时的响应
};
#endif // WIDGET_H
