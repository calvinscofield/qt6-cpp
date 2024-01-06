#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tfloatspindelegate.h"
#include "tspinboxdelegate.h"
#include "tcomboboxdelegate.h"

#include <QItemSelectionModel>
#include <QLabel>
#include <QMainWindow>
#include <QStandardItemModel>

#define FixedColumnCount 6 // 文件固定为6列

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // 用于状态栏的信息显示
    QLabel *labCurFile; // 当前文件
    QLabel *labCellPos; // 当前单元格行列号
    QLabel *labCellText; // 当前单元格内容
    QStandardItemModel *m_model; // 数据模型
    QItemSelectionModel *m_selection; // 选择模型
    void iniModelData(QStringList &aFileContent); // 从StringList初始化数据模型内容
    TSpinBoxDelegate *intSpinDelegate; // 用于编辑整数
    TFloatSpinDelegate *floatSpinDelegate; // 用于编辑浮点数
    TComboBoxDelegate *comboDelegate; // 用于列表选择
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    // 自定义槽函数，与QItemSelectionModel的currentChanged()信号连接
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_actOpen_triggered();

    void on_actAppend_triggered();

    void on_actDelete_triggered();

    void on_actAlignLeft_triggered();

    void on_actFontBold_triggered(bool checked);

    void on_actModelData_triggered();

    void on_actInsert_triggered();

    void on_actSave_triggered();

    void on_actAlignCenter_triggered();

    void on_actAlignRight_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
