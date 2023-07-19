#include "tdialogdata.h"
#include "ui_tdialogdata.h"

#include    <QFileDialog>

TDialogData::TDialogData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogData)
{
    ui->setupUi(this);
}

TDialogData::~TDialogData()
{
    delete ui;
}

//编辑记录，更新记录数据到界面
void TDialogData::setUpdateRecord(QSqlRecord &recData)
{
    m_record=recData;    //记录存入私有变量
    ui->spinEmpNo->setEnabled(false);   //员工编号不允许编辑
    setWindowTitle("更新记录");

    //根据recData的数据更新界面显示
    ui->spinEmpNo->setValue(recData.value("empNo").toInt());
    ui->editName->setText(recData.value("Name").toString());
    ui->comboSex->setCurrentText(recData.value("Gender").toString());
    ui->editBirth->setDate(recData.value("Birthday").toDate());
    ui->comboProvince->setCurrentText(recData.value("Province").toString());
    ui->comboDep->setCurrentText(recData.value("Department").toString());
    ui->spinSalary->setValue(recData.value("Salary").toInt());
    ui->editMemo->setPlainText(recData.value("Memo").toString());

    QVariant    va=recData.value("Photo");
    if (!va.isValid())  //图片字段内容为空
        ui->LabPhoto->clear();
    else    //显示图片
    {
        QByteArray data=va.toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->LabPhoto->setPixmap(pic.scaledToWidth(ui->LabPhoto->size().width()));
    }
}

//插入记录，无需更新界面显示，但是要存储recData的字段结构
void TDialogData::setInsertRecord(QSqlRecord &recData)
{
    m_record=recData;    //保存recData到私有变量
    ui->spinEmpNo->setEnabled(true);    //插入的记录，员工编号允许编辑
    setWindowTitle("插入新记录");
    ui->spinEmpNo->setValue(recData.value("empNo").toInt());
}

//点击"确定"按钮后，界面数据保存到记录变量mRecord
QSqlRecord TDialogData::getRecordData()
{
    m_record.setValue("empNo",   ui->spinEmpNo->value());
    m_record.setValue("Name",    ui->editName->text());
    m_record.setValue("Gender",  ui->comboSex->currentText());
    m_record.setValue("Birthday",ui->editBirth->date());

    m_record.setValue("Province",    ui->comboProvince->currentText());
    m_record.setValue("Department",  ui->comboDep->currentText());

    m_record.setValue("Salary",  ui->spinSalary->value());
    m_record.setValue("Memo",    ui->editMemo->toPlainText());
    //照片编辑时已经修改了mRecord的photo字段的值

    return  m_record; //以记录作为返回值
}

void TDialogData::on_btnClearPhoto_clicked()
{ //清除照片
    ui->LabPhoto->clear();
    m_record.setNull("Photo");   //Photo字段清空
}

void TDialogData::on_btnSetPhoto_clicked()
{//设置照片
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","", "照片(*.jpg)");
    if (aFile.isEmpty())
        return;

    QByteArray data;
    QFile* file=new QFile(aFile);
    file->open(QIODevice::ReadOnly);
    data = file->readAll();
    file->close();

    m_record.setValue("Photo",data); //图片保存到Photo字段

    QPixmap pic;
    pic.loadFromData(data);
    ui->LabPhoto->setPixmap(pic.scaledToWidth(ui->LabPhoto->size().width()));
}
