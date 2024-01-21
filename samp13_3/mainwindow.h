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
    QWidget *graphContainer; // 三维图的容器
    Q3DSurface *graph3D; // 三维图
    QSurface3DSeries *series; // 序列
    void iniGraph3D(); // 初始化绘制三维图

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_pointSelected(const QPoint &position);
    void on_actPoint_Modify_triggered();

    void on_actPoint_DeleteRow_triggered();

    void on_comboDrawMode_currentIndexChanged(int index);

    void on_chkBoxFlatShading_clicked(bool checked);

    void on_actSurf_Color_triggered();

    void on_actSurf_GradColor1_triggered();

    void on_actSurf_GradColor2_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
