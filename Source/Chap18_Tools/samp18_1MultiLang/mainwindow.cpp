#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCharFormat>
#include    <QSettings>
#include    <QTranslator>

extern QTranslator trans;      //必须要声明此外部变量


void MainWindow::buildUI()
{
//创建状态栏上的组件
    labFile=new QLabel(this);  //用于显示当前文件名的标签
    labFile->setMinimumWidth(150);
    labFile->setText(tr("当前文件"));
    ui->statusBar->addWidget(labFile);  //添加到状态栏

    progressBar1=new QProgressBar(this);      //状态栏上的进度条
    progressBar1->setMaximumWidth(200);
    progressBar1->setMinimum(5);
    progressBar1->setMaximum(50);
    progressBar1->setValue(ui->textEdit->font().pointSize());
    ui->statusBar->addWidget(progressBar1); //添加到状态栏

    labInfo=new QLabel(this);  //用于显示字体名称的标签
    labInfo->setText(tr("字体名称"));
    ui->statusBar->addPermanentWidget(labInfo); //添加到状态栏

//为actLang_CN和actLang_EN创建 ActionGroup，实现互斥选择
    actionGroup = new QActionGroup(this); //Action分组
    actionGroup->addAction(ui->actLang_CN);
    actionGroup->addAction(ui->actLang_EN);
    actionGroup->setExclusive(true);        //互斥性
    ui->actLang_CN->setChecked(true);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buildUI();  //动态创建界面组件

//    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);//工具栏只显示图标
    this->setCentralWidget(ui->textEdit);   //textEdit填充满工作区
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLanguage(QString curLan)
{
    if (curLan == "CN")
        ui->actLang_CN->setChecked(true);
    else
        ui->actLang_EN->setChecked(true);
}

//copyAvailable()信号，更新3个Actions的使能状态
void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actEdit_Cut->setEnabled(b);
    ui->actEdit_Copy->setEnabled(b);
    ui->actEdit_Paste->setEnabled(ui->textEdit->canPaste());
}


//粗体
void MainWindow::on_actFont_Bold_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();
    if(checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);

    ui->textEdit->mergeCurrentCharFormat(fmt);
}

//斜体
void MainWindow::on_actFont_Italic_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();
    fmt.setFontItalic(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

//下划线
void MainWindow::on_actFont_UnderLine_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();
    fmt.setFontUnderline(checked);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

//selectionChanged()信号,更新3种字体样式的checked状态
void MainWindow::on_textEdit_selectionChanged()
{
    QTextCharFormat fmt;
    fmt=ui->textEdit->currentCharFormat();

    ui->actFont_Bold->setChecked(fmt.font().bold());    //粗体

    ui->actFont_Italic->setChecked(fmt.fontItalic());   //斜体

    ui->actFont_UnderLine->setChecked(fmt.fontUnderline()); //下划线
}

//是否显示工具栏按钮文字
void MainWindow::on_actSys_ToggleText_triggered(bool checked)
{
    if (checked)
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    else
        ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

//新建
void MainWindow::on_actFile_New_triggered()
{
    ui->textEdit->clear();
    ui->textEdit->document()->setModified(false);   //表示已经保存过了,改变修改状态
    labFile->setText(tr("新建了一个文件"));
}

//打开
void MainWindow::on_actFile_Open_triggered()
{
    labFile->setText(tr("文件被打开"));
}

//保存
void MainWindow::on_actFile_Save_triggered()
{
    ui->textEdit->document()->setModified(false);   //表示已经保存过了,改变状态
    labFile->setText(tr("文件已保存"));
}

void MainWindow::on_actLang_CN_triggered()
{//切换到中文界面
    if (trans.load("samp18_1_cn.qm"))
    {
        ui->retranslateUi(this);          //重新翻译界面文字
        labInfo->setText(tr("字体名称"));
        QSettings   settings;
        settings.setValue("Language","CN");
    }
}

void MainWindow::on_actLang_EN_triggered()
{//切换到英文界面
    if (trans.load("samp18_1_en.qm"))
    {
        ui->retranslateUi(this);         //重新翻译界面文字
//        labInfo的text未再被修改过，所以labInfo的文字不会被翻译
        labInfo->setText(tr("字体名称"));
        QSettings   settings;   //自动使用QApplication::organizationName(),QApplication::applicationName()
        settings.setValue("Language","EN");
    }
}


