#ifndef TDIALOGLOGIN_H
#define TDIALOGLOGIN_H

#include <QDialog>

namespace Ui {
class TDialogLogin;
}

class TDialogLogin : public QDialog
{
    Q_OBJECT
private:
    bool m_moving = false; // 表示窗口是否在鼠标操作下移动
    QPoint m_lastPos; // 上一次的鼠标光标位置
    QString m_user = "user"; // 初始化用户名
    QString m_pswd = "12345"; // 初始化密码，未加密的
    int m_tryCount = 0; // 试错次数
    void readSettings(); // 读取设置
    void writeSettings(); // 写入设置
    QString encrypt(const QString &str); // 字符串加密

public:
    explicit TDialogLogin(QWidget *parent = nullptr);
    ~TDialogLogin();

private:
    Ui::TDialogLogin *ui;

    // QWidget interface
protected:
    // 鼠标事件处理函数，用于拖动窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_btnOK_clicked();
};

#endif // TDIALOGLOGIN_H
