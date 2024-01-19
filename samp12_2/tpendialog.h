#ifndef TPENDIALOG_H
#define TPENDIALOG_H

#include <QDialog>
#include <QPen>

namespace Ui {
class TPenDialog;
}

class TPenDialog : public QDialog
{
    Q_OBJECT
private:
    QPen m_pen;

public:
    explicit TPenDialog(QWidget *parent = nullptr);
    ~TPenDialog();
    void setPen(QPen &pen); // 设置QPen对象
    QPen getPen(); // 获取对话框设置的QPen的属性
    static QPen getPen(QPen &iniPen, bool *ok); // 静态函数

private slots:
    void on_btnColor_clicked(); // 设置颜色按钮的槽函数

private:
    Ui::TPenDialog *ui;
};

#endif // TPENDIALOG_H
