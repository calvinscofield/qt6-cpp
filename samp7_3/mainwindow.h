#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event); // 绘制主窗口背景图片
private slots:
    void do_changeTabTitle(QString title); // 用于设置tabWidget当前页的标题
    void on_actWidget_triggered();
    void on_actWindowInsite_triggered();
    void on_actWindow_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actWidgetInsite_triggered();
};
#endif // MAINWINDOW_H
