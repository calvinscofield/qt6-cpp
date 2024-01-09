#include "mainwindow.h"
#include "tformdoc.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mdiArea->setTabsClosable(true); // 页面可关闭
    ui->mdiArea->setTabsMovable(true); // 页面可移动
    setCentralWidget(ui->mdiArea);
    setWindowState(Qt::WindowMaximized); // 窗口最大化显示
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actDoc_New_triggered()
{
    TFormDoc *formDoc = new TFormDoc(this);
    ui->mdiArea->addSubWindow(formDoc); // 文档窗口添加到MDI子窗口管理器中
    formDoc->show();
    ui->actCut->setEnabled(true);
    ui->actCopy->setEnabled(true);
    ui->actPaste->setEnabled(true);
    ui->actFont->setEnabled(true);
}


void MainWindow::on_actDoc_Open_triggered()
{
    bool needNew = false; // 是否需要新建子窗口
    TFormDoc *formDoc;
    if (ui->mdiArea->subWindowList().size() > 0) // 如果有子窗口，获取活动子窗口
    {
        formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
        needNew = formDoc->isFileOpened(); // 子窗口已打开文件，需要新建子窗口
    }
    else
        needNew = true;

    QString curPath = QDir::currentPath();
    QString aFileName = QFileDialog::getOpenFileName(this, tr("打开一个文件"), curPath, "C程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)");
    if (aFileName.isEmpty())
        return;
    if (needNew) // 需要新建子窗口
    {
        formDoc = new TFormDoc(this);
        ui->mdiArea->addSubWindow(formDoc);
    }
    formDoc->loadFromFile(aFileName); // 打开文件
    formDoc->show();
    ui->actCut->setEnabled(true);
    ui->actCopy->setEnabled(true);
    ui->actPaste->setEnabled(true);
    ui->actFont->setEnabled(true);
}


void MainWindow::on_actCloseALL_triggered()
{
    // “关闭全部”按钮
    ui->mdiArea->closeActiveSubWindow();
}


void MainWindow::on_actCascade_triggered()
{
    // “级联展开”按钮
    ui->mdiArea->cascadeSubWindows();
}


void MainWindow::on_actTile_triggered()
{
    // “平铺展开”按钮
    ui->mdiArea->tileSubWindows();
}


void MainWindow::on_actViewMode_triggered(bool checked)
{
    // “MDI模式”按钮
    if (checked)
        ui->mdiArea->setViewMode(QMdiArea::TabbedView); // 多页模式
    else
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView); // 子窗口模式
    ui->mdiArea->setTabsClosable(checked); // 切换到多页模式下需重新设置
    ui->actCascade->setEnabled(!checked); // 子窗口模式下才有用
    ui->actTile->setEnabled(!checked);
}


void MainWindow::on_actDoc_Save_triggered()
{
    // “保存”按钮
    TFormDoc *formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
    formDoc->saveToFile();
}


void MainWindow::on_actCut_triggered()
{
    // “剪切”按钮
    TFormDoc *formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textCut();
}


void MainWindow::on_actCopy_triggered()
{
    // “复制”按钮
    TFormDoc *formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textCopy();
}


void MainWindow::on_actPaste_triggered()
{
    // “粘贴”按钮
    TFormDoc *formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
    formDoc->textPaste();
}


void MainWindow::on_actFont_triggered()
{
    // “字体设置”按钮
    TFormDoc *formDoc = (TFormDoc *)ui->mdiArea->activeSubWindow()->widget();
    formDoc->setEditFont();
}


void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    if (ui->mdiArea->subWindowList().size() == 0) // 若子窗口个数为零
    {
        ui->actCut->setEnabled(false);
        ui->actCopy->setEnabled(false);
        ui->actPaste->setEnabled(false);
        ui->actFont->setEnabled(false);
        ui->statusBar->clearMessage(); // 清除状态栏信息
    }
    else
    {
        TFormDoc *formDoc = static_cast<TFormDoc *>(arg1->widget());
        ui->statusBar->showMessage(formDoc->currentFileName()); // 显示子窗口的文件名
    }
}

