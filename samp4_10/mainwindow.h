#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QFontComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *labFile; // 添加到状态栏里
    QLabel *labInfo; // 添加到状态栏里
    QProgressBar *progressBar1; // 进度条，添加到状态栏里
    QSpinBox *spinFontSize; // 字体大小，添加到工具栏上
    QFontComboBox *comboFontName; // 字体名称，添加到工具栏上
    QActionGroup *actionGroup; // Action分组，用于“汉语”和“English”两个工具按钮的互斥选择
    void buildUI(); // 以代码化方式创建UI
    void buildSignalSlots(); // 手动关联信号与槽
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_fontSize_changed(int fontSize); // 改变字体大小的SpinBox的响应
    void do_fontSelected(const QFont &font); // 选择字体的FontComboBox的响应

    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_Italic_triggered(bool checked);

    void on_actFont_UnderLine_triggered(bool checked);

    void on_actFile_Save_triggered();

    void on_actSys_ToggleText_triggered(bool checked);

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_selectionChanged();

    void on_actFile_Open_triggered();

    void on_actFile_New_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
