#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tdialogheaders.h"

#include <QItemSelectionModel>
#include <QLabel>
#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *labCellPos; // 当前单元格行列号，在状态栏上显示
    QLabel *labCellText; // 当前单元格内容，在状态栏上显示
    QStandardItemModel *m_model; // 数据模型
    QItemSelectionModel *m_selection; // 选择模型
    TDialogHeaders *dlgSetHeaders = nullptr; // 设置表头标题对话框，创建一次，重复调用
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event); // 事件处理函数，关闭窗口时，询问是否退出

private slots:
    void do_model_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_actTab_SetSize_triggered();

    void on_actTab_SetHeader_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_actTab_Locate_triggered();

public slots: // 自定义共有槽函数
    void do_setCellText(int row, int column, QString &text); // 设置一个单元格的内容
signals: // 自定义信号，在tableView上点击时发射此信号
    void cellIndexChanged(int rowNo, int colNo); // 当前单元格发生变化
};
#endif // MAINWINDOW_H
