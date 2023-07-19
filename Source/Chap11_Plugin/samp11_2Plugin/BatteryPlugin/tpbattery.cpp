#include    "tpbattery.h"

#include    <QPainter>
#include    <QPaintEvent>

void TPBattery::paintEvent(QPaintEvent *event)
{  //绘制界面组件
    QPainter    painter(this);
    QRect rect(0,0,width(),height());   //viewport矩形区
    painter.setViewport(rect);          //设置Viewport
    painter.setWindow(0,0,120,50);      // 设置窗口大小，逻辑坐标
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    //绘制电池边框
    QPen  pen(colorBorder);      //划线颜色
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);    //线的类型，实线、虚线等
    pen.setCapStyle(Qt::FlatCap);   //线端点样式
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    QBrush  brush(colorBack);
    brush.setStyle(Qt::SolidPattern); //画刷填充样式
    painter.setBrush(brush);

    rect.setRect(1,1,109,48);
    painter.drawRect(rect);      //绘制电池边框

    brush.setColor(colorBorder); //画刷颜色
    painter.setBrush(brush);
    rect.setRect(110,15,10,20);
    painter.drawRect(rect);      //画电池正极头

    //画电量柱
    if (m_powerLevel>m_warnLevel) //正常颜色电量柱
    {
        brush.setColor(colorPower);
        pen.setColor(colorPower);
    }
    else        //电量低电量柱
    {
        brush.setColor(colorWarning);
        pen.setColor(colorWarning);
    }
    painter.setBrush(brush);
    painter.setPen(pen);
    if (m_powerLevel>0)
    {
        rect.setRect(5,5,m_powerLevel,40);
        painter.drawRect(rect);     //画电量柱
    }

    //绘制电量百分比文字
    QFontMetrics  textSize(this->font());
    QString powStr=QString::asprintf("%d%%",m_powerLevel);
    QRect textRect=textSize.boundingRect(powStr);   //得到字符串的矩形区
    painter.setFont(this->font());
    pen.setColor(colorBorder);
    painter.setPen(pen);
    painter.drawText(55-textRect.width()/2, 23+textRect.height()/2,  powStr);

    event->accept();        //表示事件已处理
}

TPBattery::TPBattery(QWidget *parent) : QWidget{parent}
{
    //    setPalette(QPalette(mColorBack));
    //    setAutoFillBackground(true);
    //    this->resize(120,50);
}

void TPBattery::setPowerLevel(int pow)
{ //设置当前电量值
    m_powerLevel=pow;
    emit powerLevelChanged(pow); //发射信号
    repaint();
}

int TPBattery::powerLevel()
{ //返回当前电量值
    return m_powerLevel;
}

void TPBattery::setWarnLevel(int warn)
{//设置电量低阈值
    m_warnLevel=warn;
    repaint();
}

int TPBattery::warnLevel()
{//读取电量低阈值
    return  m_warnLevel;
}

QSize TPBattery::sizeHint()
{//设置组件的合适大小
    int H=this->height();
    int W=H*12/5;
    QSize   size(W,H);
    return size;
}
