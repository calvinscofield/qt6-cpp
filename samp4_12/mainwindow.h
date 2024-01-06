#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // 枚举类型TreeItemType，创建节点时用作type参数，自定义的节点类型数据必须大于1000
    enum TreeItemType {itTopItem = 1001, itGroupItem, itImageItem};
    enum TreeColNum {colItem = 0, colItemType, colDate}; // 目录树列的序号
    QLabel *labFileName; // 用于状态栏上显示文件名
    QLabel *labNodeText; // 用于状态栏上显示节点标题
    QSpinBox *spinRatio; // 用于状态栏上显示图片缩放比例
    QPixmap m_pixmap; // 当前的图片
    float m_ratio; // 当前图片缩放比例
    void buildTreeHeader(); // 创建目录树表头
    void iniTree(); // 初始化目录树
    void addFolderItem(QTreeWidgetItem *parItem, QString dirName); // 添加目录节点
    QString getFinalFolderName(const QString &fullPathName); // 提取目录名称
    void addImageItem(QTreeWidgetItem *parItem, QString aFilename); // 添加图片节点
    void displayImage(QTreeWidgetItem *item); // 显示一个图片节点关联的图片
    void changeItemCaption(QTreeWidgetItem *item); // 遍历改变节点标题
    void deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item); // 删除一个节点

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_treeFiles_itemSelectionChanged();

    void on_actAddFolder_triggered();

    void on_actAddFiles_triggered();

    void on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actDeleteItem_triggered();

    void on_actScanItems_triggered();

    void on_actZoomFitH_triggered();

    void on_actZoomIn_triggered();

    void on_actZoomRealSize_triggered();

    void on_actDockFloat_triggered(bool checked);

    void on_actDockVisible_triggered(bool checked);

    void on_dockWidget_visibilityChanged(bool visible);

    void on_dockWidget_topLevelChanged(bool topLevel);

    void on_actZoomOut_triggered();

    void on_actZoomFitW_triggered();

    void on_treeFiles_itemCollapsed(QTreeWidgetItem *item);

    void on_treeFiles_itemExpanded(QTreeWidgetItem *item);

    void on_treeFiles_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeFiles_itemPressed(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
