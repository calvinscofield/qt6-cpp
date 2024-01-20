#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#define COL_NAME 0 // 姓名列的编号
#define COL_MATH 1 // 数学列的编号
#define COL_CHINESE 2 // 语文列的编号
#define COL_ENGLISH 3 // 英语列的编号
#define COL_AVERAGE 4 // 平均分列的编号
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int studCount = 10; // 学生人数
    QStandardItemModel *dataModel; // 数据模型
    void generateData(); // 初始化数据
    void countData(); // 统计数据
    void removeAllAxis(QChart *chart); // 删除某个chart的所有坐标轴

    void iniBarChart(); // 柱状图初始化
    void drawBarChart(bool isVertical = true);
    void iniStackedBar(); // 堆叠柱状图初始化
    void drawStackedBar(bool isVertical = true);
    void iniPercentBar(); // 百分比柱状图初始化
    void drawPercentBar(bool isVertical = true);
    void iniPieChart(); // 饼图初始化
    void drawPieChart();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_calcuAverage(QStandardItem *item); // 3个分数列的数据发生变化，重新计算平均分
    void do_barHovered(bool status, int index, QBarSet *barset);
    void do_barClicked(int index, QBarSet *barset);
    void do_pieHovered(QPieSlice *slice, bool state);

    void on_btnBuildBarChart_clicked();

    void on_btnBuildBarChartH_clicked();

    void on_btnBuildStackedBar_clicked();

    void on_btnBuildStackedBarH_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
