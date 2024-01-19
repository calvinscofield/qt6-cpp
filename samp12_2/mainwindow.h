#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QChart *chart; // 当前的图表
    QLineSeries *curSeries; // 当前序列
    QValueAxis *curAxis; // 当前坐标轴
    void createChart(); // 创建图表
    void prepareData(); // 更新数据
    void updateFromChart(); // 从图表更新到界面

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnTitleSetText_clicked();

    void on_btnTitleColor_clicked();

    void on_btnTitleFont_clicked();

    void on_groupBox_Legend_clicked(bool checked);

    void on_radioButton_clicked();

    void on_btnLegendlabelColor_clicked();

    void on_btnLegendFont_clicked();

    void on_btnSetMargin_clicked();

    void on_comboAnimation_currentIndexChanged(int index);

    void on_comboTheme_currentIndexChanged(int index);

    void on_radioSeries0_clicked();

    void on_radioSeries1_clicked();

    void on_groupBox_Series_clicked(bool checked);

    void on_btnSeriesColor_clicked();

    void on_btnSeriesPen_clicked();

    void on_chkkBoxUseOpenGL_clicked(bool checked);

    void on_groupBox_Points_clicked(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_btnPointColor_clicked();

    void on_groupBox_PointLabel_clicked(bool checked);

    void on_radioSeriesLabFormat0_clicked();

    void on_radioSeriesLabFormat1_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
