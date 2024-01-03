#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buildUI(); // 动态创建界面组件
    buildSignalSlots(); // 为动态创建的组建关联信号与槽
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly); // 工具按钮只显示图标
    this->setCentralWidget(ui->textEdit); // textEdit填充满工作区
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_fontSize_changed(int fontSize)
{
    // 设置字体大小的SpinBox
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontPointSize(fontSize); // 设置字体大小
    ui->textEdit->mergeCurrentCharFormat(fmt);
    progressBar1->setValue(fontSize); // 状态栏上显示
}

void MainWindow::do_fontSelected(const QFont &font)
{
    // 选择字体的FontComboBox
    labInfo->setText("字体名称：" + font.family()); // 状态栏上显示
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFont(font);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void MainWindow::buildUI()
{
    // 创建状态栏上的组件
    labFile = new QLabel(this); // 用于显示当前文件名
    labFile->setMinimumWidth(150);
    labFile->setText("文件名：");
    ui->statusBar->addWidget(labFile); // 添加到状态栏

    progressBar1 = new QProgressBar(this); // 状态栏上的进度条
    progressBar1->setMaximumHeight(200);
    progressBar1->setMinimum(5);
    progressBar1->setMaximum(50);
    progressBar1->setValue(ui->textEdit->font().pointSize());
    ui->statusBar->addWidget(progressBar1); // 添加到状态栏

    labInfo = new QLabel(this); // 用于显示字体名称
    labInfo->setText("选择字体名称：");
    ui->statusBar->addPermanentWidget(labInfo); // 添加到状态栏

    // 为actLang_CN（汉语）和actLang_EN（英语）创建ActionGroup，实现互斥选择
    actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actLang_CN);
    actionGroup->addAction(ui->actLang_EN);
    actionGroup->setExclusive(true); // 互斥选择
    ui->actLang_CN->setChecked(true);

    // 创建工具栏上无法可视化设计的一些组件
    spinFontSize = new QSpinBox(this); // 设置字体大小的SpinBox
    spinFontSize->setMinimum(5);
    spinFontSize->setMaximum(50);
    spinFontSize->setValue(ui->textEdit->font().pointSize());
    spinFontSize->setMinimumWidth(50);
    ui->mainToolBar->addWidget(spinFontSize); // 添加到工具栏

    comboFontName = new QFontComboBox(this); // 字体下拉列表框
    comboFontName->setMinimumWidth(150);
    ui->mainToolBar->addWidget(comboFontName); // 添加到工具栏
    ui->mainToolBar->addSeparator(); // 工具栏上增加分隔条
    ui->mainToolBar->addAction(ui->actClose); // “退出”按钮
}

void MainWindow::buildSignalSlots()
{
    connect(spinFontSize, SIGNAL(valueChanged(int)), this, SLOT(do_fontSize_changed(int)));
    connect(comboFontName, &QFontComboBox::currentFontChanged, this, &MainWindow::do_fontSelected);
}

void MainWindow::on_actFont_Bold_triggered(bool checked)
{
    // 粗体
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    if (checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(fmt);;
}


void MainWindow::on_actFont_Italic_triggered(bool checked)
{
    // 斜体
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontItalic(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFont_UnderLine_triggered(bool checked)
{
    // 下划线
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    fmt.setFontUnderline(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}


void MainWindow::on_actFile_Save_triggered()
{
    // 保存文件
    ui->textEdit->document()->setModified(false); // 表示已经保存过了，改变修改状态
    labFile->setText("文件以保存"); // 状态栏上显示
}


void MainWindow::on_actSys_ToggleText_triggered(bool checked)
{
    // 是否显示工具按钮文字
    if (checked)
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    else
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

void MainWindow::on_textEdit_copyAvailable(bool b)
{
    // copyAvailable()信号的槽函数，更新3个Action的enabled状态
    ui->actEdit_Cut->setEnabled(b);
    ui->actEdit_Copy->setEnabled(b);
    ui->actEdit_Paste->setEnabled(ui->textEdit->canPaste());
}


void MainWindow::on_textEdit_selectionChanged()
{
    // selectionChanged()信号的槽函数，更新3种字体样式的checked状态
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    ui->actFont_Bold->setChecked(fmt.font().bold()); // 粗体
    ui->actFont_Italic->setChecked(fmt.fontItalic()); // 斜体
    ui->actFont_UnderLine->setChecked(fmt.fontUnderline()); // 下划线
}


void MainWindow::on_actFile_Open_triggered()
{
    // 打开
    labFile->setText("打开的文件");
}


void MainWindow::on_actFile_New_triggered()
{
    // 新建
    ui->textEdit->clear();
    ui->textEdit->document()->setModified(false); // 表示已经保存过了，改变修改状态
    labFile->setText("新建了一个文件");
}

