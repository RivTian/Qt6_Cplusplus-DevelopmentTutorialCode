#include "mainwindow.h"
#include "ui_mainwindow.h"

#include  <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //将分割布局的垂直方向尺寸策略也设置为扩展的
    ui->splitterMain->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_model=new QFileSystemModel(this);
    m_model->setRootPath(QDir::currentPath());    //设置根目录

    ui->treeView->setModel(m_model);  //设置数据模型
    ui->listView->setModel(m_model);  //设置数据模型
    ui->tableView->setModel(m_model); //设置数据模型

    //信号与槽关联，treeView单击时，其当前节点设置为listView和tableView的根节点
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
            ui->listView,SLOT(setRootIndex(QModelIndex)));

    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),
            ui->tableView,SLOT(setRootIndex(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->chkIsDir->setChecked(m_model->isDir(index));
    ui->labPath->setText(m_model->filePath(index));
    ui->labType->setText(m_model->type(index));

    ui->labFileName->setText(m_model->fileName(index));

    int sz=m_model->size(index)/1024;
    if (sz<1024)
        ui->labFileSize->setText(QString("%1 KB").arg(sz));
    else
        ui->labFileSize->setText(QString::asprintf("%.1f MB",sz/1024.0));
}

//“设置根目录”按钮关联的Action
void MainWindow::on_actSetRoot_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择目录", QDir::currentPath());
    if (!dir.isEmpty())
    {
        m_model->setRootPath(dir);
        ui->treeView->setRootIndex(m_model->index(dir)); //设置根目录
    }
}


//“应用”按钮，应用文件名过滤
void MainWindow::on_btnApplyFilters_clicked()
{
    QString  flts=ui->comboFilters->currentText().trimmed();
    QStringList  filter=flts.split(";",Qt::SkipEmptyParts);
    m_model->setNameFilters(filter);
}

//“文件名过滤” CheckBox
void MainWindow::on_chkBoxEnableFilter_clicked(bool checked)
{//若checked==true，隐藏被过滤掉的文件节点,否则只是disabled
    m_model->setNameFilterDisables(!checked);
    ui->comboFilters->setEnabled(checked);
    ui->btnApplyFilters->setEnabled(checked);
}


//“显示目录和文件” RadioButton
void MainWindow::on_radioShowAll_clicked()
{
    ui->groupBoxFilter->setEnabled(true);
    m_model->setFilter(QDir::AllDirs | QDir::Files |QDir::NoDotAndDotDot);
}

//“只显示目录” RadioButton
void MainWindow::on_radioShowOnlyDir_clicked()
{
    ui->groupBoxFilter->setEnabled(false);
    m_model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot); //不列出文件
}


