#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWidget *graphContainer; // 图表的容器
    Q3DScatter *graph3D; // 散点图
    QScatter3DSeries *series; // 散点图序列
    void iniGraph3D(); // 初始化绘图
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_itemSelected(int index); // 与series的selectedItemChanged()信号关联
    void on_chkBoxShadow_clicked(bool checked);

    void on_spinItemSize_valueChanged(double arg1);

    void on_actPoint_ChangeValue_triggered();

    void on_actData_Add_triggered();

    void on_actData_Delete_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
