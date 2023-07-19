#include "widget.h"
#include "ui_widget.h"

#include    <QPainter>
#include    <QPaintEvent>

//绘制一个填充矩形
void Widget::myDrawFilledRect()
{
    QPainter    painter(this);  //创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int W=this->width();    //绘图区宽度
    int H=this->height();   //绘图区高度
    QRect   rect(W/4,H/4,W/2,H/2);  //中间区域矩形框

    //设置画笔
    QPen    pen;
    pen.setWidth(3);        //线宽
    pen.setColor(Qt::red);  //划线颜色
    //Qt::NoPen,Qt::SolidLine, Qt::DashLine, Qt::DotLine,Qt::DashDotLine,Qt::DashDotDotLine,Qt::CustomDashLine
    pen.setStyle(Qt::SolidLine);    //线的样式，实线、虚线等
    //Qt::FlatCap, Qt::SquareCap,Qt::RoundCap
    pen.setCapStyle(Qt::FlatCap);   //线端点样式
    //Qt::MiterJoin,Qt::BevelJoin,Qt::RoundJoin,Qt::SvgMiterJoin
    pen.setJoinStyle(Qt::BevelJoin);    //线的连接点样式
    painter.setPen(pen);

    //设置画刷
    //    QPixmap texturePixmap(":images/images/texture.jpg");
    QBrush  brush;
    brush.setColor(Qt::yellow); //画刷颜色
    brush.setStyle(Qt::SolidPattern); //画刷填充样式
    //    brush.setStyle(Qt::TexturePattern); //画刷填充样式
    //    brush.setTexture(texturePixmap); //设置材质图片
    painter.setBrush(brush);

    //Qt::NoBrush,Qt::SolidPattern,Qt::Dense1Pattern,Qt::Dense2Pattern,..,Qt::Dense7Pattern,Qt::HorPattern
    // Qt::VerPattern, Qt::CrossPattern, Qt::BDiagPattern,Qt::FDiagPattern,Qt::DiagCrossPattern,
    // Qt::LinearGradientPattern,Qt::ConicalGradientPattern,Qt::RadialGradientPattern
    // Qt::TexturePattern


    //绘图
    //    painter.drawLine(rect.left(),rect.top(),rect.right(),rect.bottom());
    painter.drawRect(rect);     //填充定义的渐变区域
    //    painter.drawRect(this->rect()); //填充更大区域，会有延展效果
    //    painter.fillRect(rect,Qt::red);
    //    painter.drawRoundRect(rect);
    //    painter.drawRoundedRect(rect,40,20);

    //    painter.drawEllipse(rect);
    //    painter.drawPolyline(points,9);
    //    painter.drawPolygon(points,9);
    //    painter.drawPoints(points,9);

    //    painter.translate(100,300);
    //    painter.drawPath(starPath);

    //    painter.translate(200,0);
    //    painter.drawPath(starPath);

    //    painter.translate(200,0);
    //    painter.drawPath(starPath);

    //    painter.drawArc(rect,30*16,80*16);
    //    painter.drawChord(rect,90*16,120*16);
    //    painter.drawPie(rect,90*16,120*16);
    //    painter.drawText(rect,"Hello, QT");
}

void Widget::myDrawTextureRect()
{
    QPainter    painter(this);      //创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int W=this->width();        //绘图区宽度
    int H=this->height();       //绘图区高度
    QRect   rect(W/4,H/4,W/2,H/2);  //中间区域矩形框


    //设置画笔
    QPen    pen;
    pen.setWidth(3);        //线宽
    pen.setColor(Qt::red);  //划线颜色
    pen.setStyle(Qt::SolidLine);        //线的类型，实线、虚线等
    pen.setCapStyle(Qt::FlatCap);       //线端点样式
    pen.setJoinStyle(Qt::BevelJoin);    //线的连接点样式
    painter.setPen(pen);

    //设置画刷
    QPixmap texturePixmap(":images/images/texture.jpg");
    QBrush  brush;
    //   brush.setColor(Qt::yellow); //画刷颜色
    //   brush.setStyle(Qt::SolidPattern); //画刷填充样式
    brush.setStyle(Qt::TexturePattern);     //画刷填充样式
    brush.setTexture(texturePixmap);        //设置材质图片
    painter.setBrush(brush);

    //绘图
    //    painter.fillRect(rect,brush);
    painter.drawRect(rect);
}

void Widget::myDrawGradient()
{
    QPainter    painter(this);  //创建QPainter对象
    //    painter.setRenderHint(QPainter::Antialiasing);
    //    painter.setRenderHint(QPainter::TextAntialiasing);

    int W=this->width();
    int H=this->height();
    QRect   rect(W/4,H/4,W/2,H/2);

    //设置画笔
    //    QPen    pen;
    //    pen.setStyle(Qt::NoPen);
    //    painter.setPen(pen);
    painter.setPen(QPen(Qt::NoPen));    //设置画笔，不显示线条


    //线性渐变
////    QLinearGradient  linearGrad(rect.left(),rect.top(),rect.right(),rect.bottom()); //对角线
//        QLinearGradient  linearGrad(rect.left(),rect.top(),rect.right(),rect.top());    //从左到右
//    linearGrad.setColorAt(0,Qt::blue);      //起点颜色
//    linearGrad.setColorAt(0.5,Qt::white);   //起点颜色
//    linearGrad.setColorAt(1,Qt::blue);      //终点颜色
//    //    linearGrad.setSpread(QGradient::ReflectSpread);  //展布模式
//    //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
//    painter.setBrush(linearGrad);

    //    //辐射渐变
        QRadialGradient  radialGrad(W/2,H/2,qMax(W/3,H/3),W/2,H/2);
        radialGrad.setColorAt(0,Qt::white);
        //        radialGrad.setColorAt(0,Qt::green);
        radialGrad.setColorAt(1,Qt::blue);
        //        radialGrad.setSpread(QGradient::ReflectSpread);
        //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
        painter.setBrush(radialGrad);

    //辐射渐变,焦点不同
    //    QRadialGradient  radialGrad(W/2,H/2,W/2,3*H/4,H/2);
    //    radialGrad.setColorAt(0,Qt::yellow);
    //    radialGrad.setColorAt(0.8,Qt::blue);
    //    //        radialGrad.setSpread(QGradient::ReflectSpread);
    //    //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
    //    painter.setBrush(radialGrad);
    //    painter.drawEllipse(rect);



    //圆锥型渐变
//    QConicalGradient  coniGrad(W/2,H/2,45);
//    coniGrad.setColorAt(0,Qt::blue);  //
//    //    coniGrad.setColorAt(0.5,Qt::blue);
//    coniGrad.setColorAt(1,Qt::white);
//    //    coniGrad.setSpread(QGradient::PadSpread); //对于锥形渐变不起作用
//    painter.setBrush(coniGrad);


    //绘图
    painter.drawRect(rect); //只填充定义的渐变区域
    //    painter.drawRect(this->rect()); //填充更大区域，会有延展效果
}

void Widget::myDrawGradientSpread()
{
    QPainter    painter(this);  //创建QPainter对象
    //    painter.setRenderHint(QPainter::Antialiasing);

    int W=this->width();
    int H=this->height();
    //    QRect   rect(W/4,H/4,W/2,H/2);
    painter.setPen(QPen(Qt::NoPen));    //设置画笔

    //辐射渐变
    QRadialGradient  radialGrad(W/2,H/2,qMax(W/8,H/8),W/2,H/2);
    radialGrad.setColorAt(0,Qt::blue);
    //        radialGrad.setColorAt(0,Qt::green);
    radialGrad.setColorAt(1,Qt::white);
    radialGrad.setSpread(QGradient::PadSpread);
    //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
    painter.setBrush(radialGrad);

    //绘图
    //    painter.drawRect(rect); //只填充定义的渐变区域
    painter.drawRect(this->rect()); //填充更大区域，会有延展效果

}

void Widget::total()
{
    QPainter    painter(this);//创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);//
    painter.setRenderHint(QPainter::TextAntialiasing);//

    int W=this->width(); //绘图区宽度
    int H=this->height(); //绘图区高度
    QRect   rect(W/4,H/4,W/2,H/2); //中间区域矩形框

    //    QPoint points[9]={
    //        QPoint(5*W/12,H/4),
    //        QPoint(7*W/12,H/4),
    //        QPoint(3*W/4,5*H/12),
    //        QPoint(3*W/4,7*H/12),
    //        QPoint(7*W/12,3*H/4),
    //        QPoint(5*W/12,3*H/4),
    //        QPoint(W/4,7*H/12),
    //        QPoint(W/4,5*H/12),
    //        QPoint(5*W/12,H/4)
    //    };

    //    QPainterPath  path;
    //    path.addRect(rect);
    //    path.addEllipse(rect);
    //    path.moveTo(points[0]);
    //    path.lineTo(points[4]);
    //    path.moveTo(points[1]);
    //    path.lineTo(points[5]);
    //    path.moveTo(points[2]);
    //    path.lineTo(points[6]);
    //    path.moveTo(points[3]);
    //    path.lineTo(points[7]);

    //    painter.scale(width() / 100.0, height() / 100.0);
    //    painter.translate(50.0, 50.0);

    //    qreal   R=100; //半径
    //    const   qreal Pi=3.14159;
    //    qreal   deg=Pi*72/180;//

    //    QPoint points[5]={
    //        QPoint(R,0),
    //        QPoint(R*std::cos(deg),-R*std::sin(deg)),
    //        QPoint(R*std::cos(2*deg),-R*std::sin(2*deg)),
    //        QPoint(R*std::cos(3*deg),-R*std::sin(3*deg)),
    //        QPoint(R*std::cos(4*deg),-R*std::sin(4*deg)),
    //    };


    //    QPainterPath starPath;
    //    starPath.moveTo(points[0]);
    //    starPath.lineTo(points[2]);
    //    starPath.lineTo(points[4]);
    //    starPath.lineTo(points[1]);
    //    starPath.lineTo(points[3]);
    //    starPath.closeSubpath();

    //绘制曲线
    //    path.moveTo(points[0]);
    //    path.cubicTo(points[1],points[4],points[2]);

    //设置字体
    //    QFont   font;
    //    font.setPointSize(30);
    //    font.setBold(true);
    //    painter.setFont(font);

    //设置画笔
    QPen    pen;
    pen.setWidth(3); //线宽
    pen.setColor(Qt::red); //划线颜色

    //Qt::NoPen,Qt::SolidLine, Qt::DashLine, Qt::DotLine,Qt::DashDotLine,Qt::DashDotDotLine,Qt::CustomDashLine
    pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等

    //Qt::FlatCap, Qt::SquareCap,Qt::RoundCap
    pen.setCapStyle(Qt::FlatCap);//线端点样式

    //Qt::MiterJoin,Qt::BevelJoin,Qt::RoundJoin,Qt::SvgMiterJoin
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    //设置画刷
    //    QPixmap texturePixmap(":images/images/texture.jpg");
    QBrush  brush;
    brush.setColor(Qt::yellow); //画刷颜色
    brush.setStyle(Qt::SolidPattern); //画刷填充样式
    //    brush.setStyle(Qt::TexturePattern); //画刷填充样式
    //    brush.setTexture(texturePixmap); //设置材质图片
    painter.setBrush(brush);

    //Qt::NoBrush,Qt::SolidPattern,Qt::Dense1Pattern,Qt::Dense2Pattern,..,Qt::Dense7Pattern,Qt::HorPattern
    // Qt::VerPattern, Qt::CrossPattern, Qt::BDiagPattern,Qt::FDiagPattern,Qt::DiagCrossPattern,
    // Qt::LinearGradientPattern,Qt::ConicalGradientPattern,Qt::RadialGradientPattern
    // Qt::TexturePattern

    //线性渐变
    ////    QLinearGradient  linearGrad(rect.left(),rect.top(),rect.right(),rect.bottom()); //对角线
    //    QLinearGradient  linearGrad(rect.left(),rect.top(),rect.right(),rect.top());//从左到右
    //    linearGrad.setColorAt(0,Qt::blue);//起点颜色
    //    linearGrad.setColorAt(0.5,Qt::blue);//起点颜色
    //    linearGrad.setColorAt(1,Qt::red);//终点颜色
    //    linearGrad.setSpread(QGradient::ReflectSpread);  //展布模式
    ////QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
    //    painter.setBrush(linearGrad);

    //径向渐变
    //    QRadialGradient  radialGrad(W/2,H/2,qMax(W/8,H/8),W/2,H/2);
    ////    radialGrad.setColorAt(0,Qt::white);
    //    radialGrad.setColorAt(0,Qt::green);
    //    radialGrad.setColorAt(1,Qt::blue);
    //    radialGrad.setSpread(QGradient::ReflectSpread);
    //    //QGradient::PadSpread ,QGradient::RepeatSpread, QGradient::ReflectSpread
    //    painter.setBrush(radialGrad);


    //圆锥型渐变
    //    QConicalGradient  coniGrad(W/2,H/2,45);
    //    coniGrad.setColorAt(0,Qt::yellow);
    //    coniGrad.setColorAt(0.5,Qt::blue);
    //    coniGrad.setColorAt(1,Qt::green);
    ////    coniGrad.setSpread(QGradient::PadSpread); //对于锥形渐变不起作用
    //    painter.setBrush(coniGrad);


    //设置复合模式
    //    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//

    //    painter.translate(W/4,H/4);
    //    painter.rotate(30);
    //    painter.scale(0.5,0.5);
    //    painter.shear(0.1,0.5);

    //绘图
    //    painter.drawLine(rect.left(),rect.top(),rect.right(),rect.bottom());
    painter.drawRect(rect); //只填充定义的渐变区域
    //    painter.drawRect(this->rect()); //填充更大区域，会有延展效果
    //    painter.fillRect(rect,Qt::red);
    //    painter.drawRoundRect(rect);
    //    painter.drawRoundedRect(rect,40,20);
    //    painter.drawEllipse(rect);
    //    painter.drawPolyline(points,9);
    //    painter.drawPolygon(points,9);
    //    painter.drawPoints(points,9);

    //    painter.translate(100,300);
    //    painter.drawPath(starPath);

    //    painter.translate(200,0);
    //    painter.drawPath(starPath);

    //    painter.translate(200,0);
    //    painter.drawPath(starPath);

    //    painter.drawArc(rect,30*16,80*16);
    //    painter.drawChord(rect,90*16,120*16);
    //    painter.drawPie(rect,90*16,120*16);
    //    painter.drawText(rect,"Hello, QT");

    //    QPixmap pixmap;
    //    pixmap.load(":/images/images/qt.jpg");
    //    painter.drawPixmap(rect.left(),rect.top(),pixmap);

}

void Widget::myDrawShape()
{
    QPainter    painter(this);//创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int W=this->width(); //绘图区宽度
    int H=this->height(); //绘图区高度

    //设置画笔
    QPen    pen;
    pen.setWidth(3); //线宽
    pen.setColor(Qt::black); //划线颜色

    //Qt::NoPen,Qt::SolidLine, Qt::DashLine, Qt::DotLine,Qt::DashDotLine,Qt::DashDotDotLine,Qt::CustomDashLine
    pen.setStyle(Qt::SolidLine);//线的类型，实线、虚线等

    //Qt::FlatCap, Qt::SquareCap,Qt::RoundCap
    pen.setCapStyle(Qt::FlatCap);//线端点样式

    //Qt::MiterJoin,Qt::BevelJoin,Qt::RoundJoin,Qt::SvgMiterJoin
    pen.setJoinStyle(Qt::BevelJoin);//线的连接点样式
    painter.setPen(pen);

    //1. drawArc()
//    QRect   rect(W/4,H/4,W/2,H/2);
//    int startAngle = 90 * 16; //起始90°
//    int spanAngle = 90 * 16;   //旋转90°
//    painter.drawArc(rect, startAngle, spanAngle);

    //2. drawChord()
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   int startAngle = 90 * 16; //起始90°
    //   int spanAngle = 90 * 16;   //旋转90°
    //   painter.drawChord(rect, startAngle, spanAngle);

    //3.drawConvexPolygon()
    //   QPoint points[4]={
    //      QPoint(5*W/12,H/4),
    ////      QPoint(7*W/12,H/4),
    //      QPoint(3*W/4,5*H/12),
    ////      QPoint(3*W/4,7*H/12),
    ////      QPoint(7*W/12,3*H/4),
    //      QPoint(5*W/12,3*H/4),
    ////      QPoint(W/4,7*H/12),
    //      QPoint(W/4,5*H/12),
    ////      QPoint(5*W/12,H/4)
    //   };
    //   painter.drawConvexPolygon(points, 4);


    //4. drawEllipse
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   painter.drawEllipse(rect);

    //5. drawImage
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   QImage  image(":images/images/qt.jpg");
    //   painter.drawImage(rect, image);

    //5. drawPixmap
    //    QRect   rect(W/4,H/4,W/2,H/2);
    //    QPixmap  image(":images/images/qt.jpg");
    //    painter.drawPixmap(rect, image);


    //6.  drawLine
    //   QLine   Line(W/4,H/4,W/2,H/2);
    //   painter.drawLine(Line);

    //7.  drawLines
//       QRect   rect(W/4,H/4,W/2,H/2);
//       QList<QLine> Lines;
//       Lines.append(QLine(rect.topLeft(),rect.bottomRight()));
//       Lines.append(QLine(rect.topRight(),rect.bottomLeft()));
//       Lines.append(QLine(rect.topLeft(),rect.bottomLeft()));
//       Lines.append(QLine(rect.topRight(),rect.bottomRight()));
//       painter.drawLines(Lines);

    // 8.QPainterPath
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   QPainterPath  path;
    //   path.addEllipse(rect);
    //   path.addRect(rect);
    //   painter.drawPath(path);

    //9.drawPie
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   int startAngle = 40 * 16;//起始40°
    //   int spanAngle = 120 * 16;//旋转120°
    //   painter.drawPie(rect, startAngle, spanAngle);

    //10. drawPixmap
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   QPixmap   pixmap(":images/images/qt.jpg");
    //   painter.drawPixmap(rect, pixmap);

    //11. drawPolygon
       QPoint points[]={
          QPoint(5*W/12,H/4),
          QPoint(3*W/4,5*H/12),
          QPoint(5*W/12,3*H/4),
          QPoint(2*W/4,5*H/12),
       };
       painter.drawPolygon(points, 4);

    //12.drawPolyline
    //   QPoint points[]={
    //      QPoint(5*W/12,H/4),
    //      QPoint(3*W/4,5*H/12),
    //      QPoint(5*W/12,3*H/4),
    //      QPoint(2*W/4,5*H/12)
    //   };
    //   painter.drawPolyline(points, 4);

    //13.drawRect
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   painter.drawRect(rect);

    //14.drawRect
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   painter.drawRoundedRect(rect,20,20);

    //14.drawRect
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   QFont   font;
    //   font.setPointSize(30);
    //   font.setBold(true);
    //   painter.setFont(font);
    //   painter.drawText (rect,"Hello,Qt");

    //  eraseRect
    //   QRect   rect(W/4,H/4,W/2,H/2);
    //   painter.eraseRect(rect);

    //15. fillRect
    //    QRect   rect(W/4,H/4,W/2,H/2);
    //    painter.fillRect (rect,Qt::green);

    //16.   fillPath
    //   QRect  rect(W/4,H/4,W/2,H/2);
    //   QPainterPath  path;
    //   path.addEllipse(rect);
    //   path.addRect(rect);
    //   painter.fillPath(path,Qt::red);

    //17.drawPoint
    //     painter.drawPoint(QPoint(W/2,H/2));

    //18. drawPoints
    //     QPoint points[]={
    //        QPoint(5*W/12,H/4),
    //        QPoint(3*W/4,5*H/12),
    //        QPoint(2*W/4,5*H/12)
    //     };
    //     painter.drawPoints(points, 3);
}

void Widget::paintEvent(QPaintEvent *event)
{
    //    Q_UNUSED(event);
    // 1. 基本绘图1，绘制一个填充矩形
    myDrawFilledRect();

    // 2.   用材质图片填充一个矩形
//        myDrawTextureRect();

    //3.  渐变填充
//        myDrawGradient();
    //    myDrawGradientSpread();     //延展渐变

    //4. 绘制基本图形
//    myDrawShape();

    event->accept();
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setPalette(QPalette(Qt::white));//设置窗口为白色背景
    setAutoFillBackground(true);
    this->resize(400,300);
}

Widget::~Widget()
{
    delete ui;
}
