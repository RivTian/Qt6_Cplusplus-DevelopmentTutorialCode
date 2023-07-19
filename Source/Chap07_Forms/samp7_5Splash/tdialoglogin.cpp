#include "tdialoglogin.h"
#include "ui_tdialoglogin.h"

#include    <QMouseEvent>
#include    <QSettings>
#include    <QMessageBox>
#include    <QByteArray>
#include    <QCryptographicHash>


//读取存储的用户名和密码, 密码是经过加密的
void TDialogLogin::readSettings()
{
    QSettings settings;     //创建QSettings对象
    bool saved=settings.value("saved",false).toBool();      //读取 saved键的值
    m_user=settings.value("Username", "user").toString();   //读取 Username 键的值，默认为“user”

    QString defaultPSWD=encrypt("12345");       //默认密码"12345"加密后的数据

    m_pswd=settings.value("PSWD",defaultPSWD).toString();   //读取PSWD 键的值

    if (saved)
        ui->editUser->setText(m_user);

    ui->chkBoxSave->setChecked(saved);
}

//保存用户名，密码等设置
void TDialogLogin::writeSettings()
{
    QSettings   settings;   //注册表键组
    settings.setValue("Username",m_user);       //用户名
    settings.setValue("PSWD",m_pswd);           //密码，经过加密的
    settings.setValue("saved",ui->chkBoxSave->isChecked());
}

QString TDialogLogin::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray= str.toLocal8Bit();  //字符串转换为字节数组数据

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();       //转换为16进制字符串
    return  md5;
}

void TDialogLogin::mousePressEvent(QMouseEvent *event)
{ //鼠标按键被按下
    if (event->button() == Qt::LeftButton)
    {
        m_moving = true;
        m_lastPos = event->globalPosition().toPoint() - this->pos();
    }
    return QDialog::mousePressEvent(event);
}

void TDialogLogin::mouseMoveEvent(QMouseEvent *event)
{ //鼠标按下左键移动
    QPoint eventPos=event->globalPosition().toPoint();

    if (m_moving && (event->buttons() & Qt::LeftButton)
        && (eventPos-m_lastPos).manhattanLength() > QApplication::startDragDistance())
    {
        move(eventPos-m_lastPos);
        m_lastPos = eventPos - this->pos();
    }
    return QDialog::mouseMoveEvent(event);
}

void TDialogLogin::mouseReleaseEvent(QMouseEvent *event)
{ //鼠标按键被释放
    m_moving=false;     //停止移动
    event->accept();
}

TDialogLogin::TDialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogLogin)
{
    ui->setupUi(this);

    ui->editPSWD->setEchoMode(QLineEdit::Password);     //设置为密码输入模式
    this->setAttribute(Qt::WA_DeleteOnClose);           //对话框关闭时自动删除
    this->setWindowFlags(Qt::SplashScreen);             //设置为SplashScreen, 窗口无边框,不在任务栏显示
    //    this->setWindowFlags(Qt::FramelessWindowHint);    //无边框，但是在任务显示对话框标题

    QApplication::setOrganizationName("WWB-Qt");
    QApplication::setApplicationName("samp7_5");
    readSettings(); //读取存储的用户名和密码
}

TDialogLogin::~TDialogLogin()
{
    delete ui;
}


//"确定"按钮响应
void TDialogLogin::on_btnOK_clicked()
{
    QString user=ui->editUser->text().trimmed();    //输入的用户名
    QString pswd=ui->editPSWD->text().trimmed();    //输入的密码
    QString encrptPSWD=encrypt(pswd);   //对输入的密码进行加密

    if ((user==m_user)&&(encrptPSWD==m_pswd))       //如果用户名和密码正确
    {
        writeSettings();    //保存设置
        this->accept();     //accept()，关闭对话框
    }
    else
    {
        m_tryCount++;       //错误次数
        if (m_tryCount>3)
        {
            QMessageBox::critical(this, "错误", "输入错误次数太多，强行退出");
            this->reject(); //reject(), 关闭对话框
        }
        else
            QMessageBox::warning(this, "错误提示", "用户名或密码错误");
    }
}
