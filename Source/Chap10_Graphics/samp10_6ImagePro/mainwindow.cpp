#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QPrinter>
#include    <QPrintDialog>
#include    <QPrintPreviewDialog>
#include    <QPainter>
#include    <QTextBlock>


//formatChanged, 改变了图像格式
void MainWindow::showImageFeatures(bool formatChanged)
{
    if (formatChanged)    //格式转换后需要显示全部信息
    {
        QImage::Format fmt=m_image.format();  //图像格式
        if (fmt == QImage::Format_RGB32)
            ui->editImg_Format->setText("32-bit RGB(0xffRRGGBB)");
        else if (fmt == QImage::Format_RGB16)
            ui->editImg_Format->setText("16-bit RGB565");
        else if (fmt == QImage::Format_RGB888)
            ui->editImg_Format->setText("24-bit RGB888");
        else if (fmt == QImage::Format_Grayscale8)
            ui->editImg_Format->setText("8-bit grayscale");
        else if (fmt == QImage::Format_Grayscale16)
            ui->editImg_Format->setText("16-bit grayscale");
        else if (fmt == QImage::Format_ARGB32)
            ui->editImg_Format->setText("32-bit ARGB(0xAARRGGBB)");
        else if (fmt == QImage::Format_Indexed8)
            ui->editImg_Format->setText("8-bit indexes into a colormap");
        else
            ui->editImg_Format->setText(QString("Format= %1,其他格式").arg(fmt));

        ui->editImg_Depth->setText(QString("%1 bits/pixel").arg(m_image.depth()));
        ui->editImg_BitPlane->setText(QString("%1 bits").arg(m_image.bitPlaneCount()));

        ui->chkBox_Alpha->setChecked(m_image.hasAlphaChannel());
        ui->chkBox_GrayScale->setChecked(m_image.isGrayscale());
    }

    //缩放，或旋转之后显示大小信息
    ui->editImg_Height->setText(QString("%1 像素").arg(m_image.height()));
    ui->editImg_Width->setText(QString("%1 像素").arg(m_image.width()));

    qsizetype sz =m_image.sizeInBytes();      //图像数据字节数
    if (sz<1024*9)
        ui->editImg_SizeByte->setText(QString("%1 Bytes").arg(sz));
    else
        ui->editImg_SizeByte->setText(QString("%1 KB").arg(sz/1024));

    QString dpi=QString::asprintf("DPI_X=%.0f, DPI_Y=%.0f",
                                  m_image.dotsPerMeterX()*0.0254,m_image.dotsPerMeterY()*0.0254);
    ui->editImg_DPM->setText(dpi);  //DPI分辨率

}

void MainWindow::imageModified(bool modified)
{
    ui->actFile_Reload->setEnabled(modified);
    ui->actFile_Save->setEnabled(modified);
}

void MainWindow::printImage(QPainter *painter, QPrinter *printer)
{//打印图像
    QMargins margin(20,40,20,40);   //上下左右4个边距，单位：像素
    QRectF pageRect=printer->pageRect(QPrinter::DevicePixel);   //单位：设备像素
    int pageW=pageRect.width();     //打印页面的宽度
    int pageH=pageRect.height();

    const int lineInc=20;   //一行文字所占的行高度,单位：像素
    int curX=margin.left(); //当前X坐标
    int curY=margin.top();  //当前Y坐标

    painter->drawText(curX,curY,m_filename);   //打印图片文件名
    curY += lineInc;    //移到下一行

    painter->drawText(curX,curY,QString("Page width =%1 像素").arg(pageW));
    painter->drawText(200,curY,QString("Image width =%1 像素").arg(m_image.width()));
    curY += lineInc;

    painter->drawText(curX,curY,QString("Page height=%1 像素").arg(pageH));
    painter->drawText(200,curY,QString("Image height=%1 像素").arg(m_image.height()));
    curY += lineInc;

    int spaceH= pageH-curY;  //页面剩余的高度

    //图像未超过页面范围，居中显示实际大小的图片
    if ((pageW >m_image.width()) && (spaceH >m_image.height()))
    {
        curX =(pageW-m_image.width())/2;          //使水平居中
        painter->drawImage(curX,curY,m_image);    //打印图像
        return;
    }

    //否则图像高度或宽度超过了页面剩余空间，缩放后打印
    QImage newImg;
    if (m_image.height() > m_image.width())
        newImg =m_image.scaledToHeight(spaceH);   //按高度缩放
    else
        newImg =m_image.scaledToWidth(pageW);     //按宽度缩放
    curX =(pageW-newImg.width())/2;             //使水平居中
    painter->drawImage(curX,curY,newImg);       //打印图像
}

void MainWindow::printRGB565Data(QPainter *painter, QPrinter *printer)
{//打印文档
    QMargins margin(20,40,20,40);   //上下左右4个边距，单位：像素
    QRectF pageRect=printer->pageRect(QPrinter::DevicePixel); //单位：设备像素
    int pageW=pageRect.width();     //打印页面的宽度，像素
    int pageH=pageRect.height();

    const int lineInc=25;   //一行文字所占的行高度，单位：像素
    int curX=margin.left(); //当前X坐标
    int curY=margin.top();  //当前Y坐标

    QFont font=ui->plainText->font();
    painter->setFont(font);       //设置打印字体

    int pageNum=1;      //打印页面编号
    painter->drawLine(margin.left(), pageH- margin.bottom()+1,            //页脚划线
                   pageW-margin.right(), pageH- margin.bottom()+1);
    painter->drawText(pageW-5*margin.right(),pageH-margin.bottom()+20,    //页脚页面编号
                   QString("第 %1 页").arg(pageNum));

    QTextDocument* doc=ui->plainText->document();       //文本对象
    int cnt=doc->blockCount();      //回车符是一个block
    for (int i=0; i<cnt; i++)       //逐行读取文字，逐行打印
    {
         QTextBlock  textLine =doc->findBlockByNumber(i);   // 文本中的一段
         QString str=textLine.text();           //一行文字
         painter->drawText(curX,curY,str);      //打印文字
         curY += lineInc;           //换到下一行

         if (curY>= (pageH-margin.bottom()))    //需要换页
         {
             printer->newPage();    //新建一个打印页
             curY=margin.top();     //一页的首行位置
             pageNum++;             //页面编号++
             painter->drawLine(margin.left(), pageH- margin.bottom()+1,   //页脚划线
                            pageW-margin.right(), pageH- margin.bottom()+1);
             painter->drawText(pageW-5*margin.right(),pageH-margin.bottom()+20,
                            QString("第 %1 页").arg(pageNum));            //页脚页面编号
         }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_paintRequestedImage(QPrinter *printer)
{
    QPainter painter(printer);    //打印机的画笔
    printImage(&painter, printer);
}

void MainWindow::do_paintRequestedText(QPrinter *printer)
{
    QPainter painter(printer);    //打印机的画笔
    printRGB565Data(&painter, printer);
}

void MainWindow::on_actFile_Open_triggered()
{//”打开”按钮
    QString  curPath= QDir::currentPath();   //应用程序当前目录

    QString  filter="图片文件(*.bmp *.jpg *.png);;"
                    "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png)";

    QString  fileName=QFileDialog::getOpenFileName(this,"选择图片文件",curPath,filter);
    if (fileName.isEmpty())
        return;
    ui->statusbar->showMessage(fileName);
    m_filename=fileName;     //保存当前图片文件名

    QFileInfo  fileInfo(fileName);
    QDir::setCurrent(fileInfo.absolutePath());  //设置应用程序当前目录

    m_image.load(fileName);   //载入图片文件
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);      //在QLabel组件上显示图片
    ui->tabWidget->setCurrentIndex(0);

    showImageFeatures();    //显示图片属性

    ui->frameLeft->setEnabled(true);
    ui->actFile_SaveAs->setEnabled(true);

    ui->actImg_ZoomIn->setEnabled(true);
    ui->actImg_ZoomOut->setEnabled(true);
    ui->actImg_FlipLR->setEnabled(true);
    ui->actImg_FlipUD->setEnabled(true);
    ui->actImg_RotateLeft->setEnabled(true);
    ui->actImg_RotateRight->setEnabled(true);
    ui->actFile_Print->setEnabled(true);
    ui->actFile_Preview->setEnabled(true);
}


void MainWindow::on_actImg_RotateLeft_triggered()
{//左旋90度
    QTransform matrix;
    matrix.reset();     //单位矩阵
    matrix.rotate(-90); //默认Qt::ZAxis

    m_image=m_image.transformed(matrix);    //使用变换矩阵matrix进行图像变换
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);  //刷新界面的图像显示
    showImageFeatures(false);       //只刷新显示图像尺寸相关信息
    imageModified(true);
}

void MainWindow::on_actImg_RotateRight_triggered()
{//右旋90度
    QTransform matrix;
    matrix.reset();     //单位矩阵
    matrix.rotate(90);  //默认Qt::ZAxis

    m_image=m_image.transformed(matrix);    //使用变换矩阵matrix进行图像变换
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);      //刷新界面的图像显示
    showImageFeatures(false);           //只刷新显示图像尺寸相关信息
    imageModified(true);
}


void MainWindow::on_actImg_FlipUD_triggered()
{//上下翻转
    bool horizontal=false;
    bool vertical=true;
    m_image.mirror(horizontal,vertical);  //图像镜像处理
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);
    imageModified(true);
}


void MainWindow::on_actImg_FlipLR_triggered()
{//左右翻转
    bool horizontal=true;
    bool vertical=false;
    m_image.mirror(horizontal,vertical);  //图像镜像处理
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);
    imageModified(true);
}


void MainWindow::on_actFile_Reload_triggered()
{//重新载入
    QString fileName =m_filename;
    m_image.load(fileName);       //从当前文件重新载入
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);  //刷新图像显示
    ui->tabWidget->setCurrentIndex(0);

    showImageFeatures(true);    //显示全部属性
    imageModified(false);       //设置按钮状态
}

void MainWindow::on_btnFormatConvert_clicked()
{//图像格式转换
//    QImage newImage;
    int index=ui->comboFormat->currentIndex();
    if (index ==0)
        m_image.convertTo(QImage::Format_RGB16);      //RGB565
    else if (index ==1)
        m_image.convertTo(QImage::Format_RGB888);     //RGB888
    else if (index ==2)
        m_image.convertTo(QImage::Format_RGB32);      //RGBx888
    else if (index ==3)
//        newImage = image.convertToFormat(QImage::Format_Grayscale8);        //不改变原图
//    newImage = image.convertedTo(QImage::Format_Grayscale8);        //不改变原图像
        m_image.convertTo(QImage::Format_Grayscale8); //8位灰度
    else if (index ==4)
        m_image.convertTo(QImage::Format_Grayscale16);//16位灰度
    else if (index ==5)
        m_image.convertTo(QImage::Format_Indexed8);   //8位索引
    else
        return;

    QPixmap  pixmap=QPixmap::fromImage(m_image);      //刷新界面的图像显示
    ui->labPic->setPixmap(pixmap);
    showImageFeatures(true);    //显示全部信息
    imageModified(true);        //图像被修改了
}


void MainWindow::on_btnGetRGB565_clicked()
{
    ui->plainText->clear();

    int W=m_image.width();
    int H=m_image.height();
    int total=2*W*H;        //总数据字节数
    QFileInfo  fileInfo(m_filename);
    QString arrayName=fileInfo.baseName();  //不带后缀的文件名
    QString aLine=QString("const unsigned char RGB565_%1[%2] = {").arg(arrayName).arg(total);
    ui->plainText->appendPlainText(aLine);

    QString   onePixel; //一个像素的2字节16进制数据字符串
    QChar   ch0('0');   //用于填充的字符
    int base=16;        //16进制
    int count=0;        //单行像素个数计数
    for (int y=0; y<H; y++)     //从上到下逐行处理
    {
        QApplication::processEvents();
        for (int x=0; x<W; x++) //从左到右逐个像素处理
        {
            QRgb rgb=m_image.pixel(x,y);   //一个像素的RGB颜色, 格式 0xAARRGGBB

/////////自己处理，有效
//            quint32 tmp32 = rgb & 0x00F80000;   //取red高5位
//            rgb565 = tmp32>>8;          //Red5
//            tmp32 = rgb & 0x0000FC00;       //取green高6位
//            rgb565 = rgb565 | (tmp32>>5);   //R5G6
//            tmp32 = rgb & 0x000000F8;   //取blue高5位
//            rgb565 =rgb565 | (tmp32>>3);    //RGB565
//            quint8 byteLSB = rgb565 & 0x00FF;
//            quint8 byteMSB = rgb565>>8;

            //////使用qRed()等函数，有效
            quint16 red  =qRed(rgb)   & 0x00F8; //取高5位
            quint16 green=qGreen(rgb) & 0x00FC; //取高6位
            quint16 blue =qBlue(rgb)  & 0x00F8; //取高5位
            quint16 rgb565=(red<<8) | (green <<3) | (blue>>3);  //RGB565数据
            quint8 byteLSB = rgb565 & 0x00FF;   //低字节
            quint8 byteMSB = rgb565>>8;         //高字节

            if (ui->radioLSB->isChecked())      //低字节在前
                onePixel += QString("0x%1,0x%2,").arg(byteLSB, 2, base,ch0).arg(byteMSB, 2, base,ch0);
//            onePixel=onePixel+QString("0x%1,0x%2,").arg(byteLSB, 2, base,ch0).arg(byteMSB, 2, base,ch0);
            else
                onePixel += QString("0x%1,0x%2,").arg(byteMSB, 2, base,ch0).arg(byteLSB, 2, base,ch0);
//            onePixel=onePixel+QString("0x%1,0x%2,").arg(byteMSB, 2, base,ch0).arg(byteLSB, 2, base,ch0);

            count++;
            if (count==8)   //每行只填8个像素的数据
            {
                onePixel = onePixel.toUpper();
                onePixel = onePixel.replace(QChar('X'),"x");
                ui->plainText->appendPlainText(onePixel);

                onePixel="";
                count=0;
            }
        }
    }

    if (count>0)    //最后不足8个像素的数据
    {
        onePixel = onePixel.toUpper();
        onePixel = onePixel.replace(QChar('X'),"x");
        ui->plainText->appendPlainText(onePixel);
    }

    ui->plainText->appendPlainText("};");   //数组结尾
    ui->tabWidget->setCurrentIndex(1);
    ui->btnSaveDataFile->setEnabled(true);
    QMessageBox::information(this,"提示","RGB565数据生成已完成");
}

void MainWindow::on_btnSaveDataFile_clicked()
{
    QFileInfo fileInfo(m_filename);
    QString  newName=fileInfo.baseName()+".h";       //更改文件后缀
    QString  filter="C语言头文件(*.h);;C语言程序文件(*.c);;文本文件(*.txt)";
    QString  fileName=QFileDialog::getSaveFileName(this,"保存文件",newName,filter);
    if (fileName.isEmpty())
        return;


//    QFileInfo fileInfo(imageFilename);
//    QString  newName=fileInfo.fileName();   //去除路径的文件名
//    fileInfo.setFile(newName);
//    newName=fileInfo.baseName()+".h";       //更改文件后缀
//    newName=QDir::currentPath()+"\\"+newName;   //应用程序当前目录
//    QString  filter="C语言头文件(*.h);;C语言程序文件(*.c);;文本文件(*.txt)";
//    QString  fileName=QFileDialog::getSaveFileName(this,"保存文件",newName,filter);

//    if (fileName.isEmpty())
//        return;
//    fileInfo.setFile(fileName);
//    QDir::setCurrent(fileInfo.absolutePath());  //设置应用程序当前目录

    QFile   aFile(fileName);
    if (aFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString str=ui->plainText->toPlainText();	//整个内容作为字符串
        QByteArray  strBytes=str.toUtf8();          //转换为字节数组, UTF-8编码
        aFile.write(strBytes,strBytes.length());    //写入文件
        aFile.close();
    }
}

void MainWindow::on_actFile_Save_triggered()
{
    m_image.save(m_filename);  //保存到当前文件
    imageModified(false);
//    ui->actFile_Save->setEnabled(false);
//    ui->actFile_Reload->setEnabled(false);
}


void MainWindow::on_actFile_SaveAs_triggered()
{
//    QString  filter="图片文件(*.bmp *.jpg *.png);;BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png)";
    QString  filter= "图片文件(*.bmp *.jpg *.png);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png)";
    QString  fileName=QFileDialog::getSaveFileName(this,"保存文件",m_filename,filter);
    if (fileName.isEmpty())
        return;

    m_image.save(fileName);  //保存到新的文件
    m_filename= fileName;    //重新设置当前文件名
    ui->statusbar->showMessage(fileName);
    imageModified(false);
}


void MainWindow::on_actFile_Print_triggered()
{//"打印"按钮
    QPrinter printer;
    QPrintDialog pritnDialog(&printer,this);    //打印设置对话框

    if (pritnDialog.exec()==QDialog::Accepted)
    {
        QPainter painter(&printer);     //打印机的画笔
        if (ui->tabWidget->currentIndex() == 0)
            printImage(&painter, &printer);         //打印图像
        else
            printRGB565Data(&painter, &printer);    //打印文本
    }
}

void MainWindow::on_actFile_Preview_triggered()
{//”打印预览”按钮
    QPrintPreviewDialog  previewDlg(this);    //打印预览对话框
    previewDlg.setWindowFlag(Qt::WindowMaximizeButtonHint); //具有最大化按钮

    if (ui->tabWidget->currentIndex() == 0)
        connect(&previewDlg, SIGNAL(paintRequested(QPrinter *)),
                this, SLOT(do_paintRequestedImage(QPrinter *)));
    else
        connect(&previewDlg, SIGNAL(paintRequested(QPrinter *)),
                this, SLOT(do_paintRequestedText(QPrinter *)));

    previewDlg.exec();  //以模态方式显示对话框
}


void MainWindow::on_actImg_ZoomIn_triggered()
{//放大
    int W=m_image.width();
    int H=m_image.height();
    m_image=m_image.scaled(1.1*W, 1.1*H,Qt::KeepAspectRatio);   //放大
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);      //重新设置pixmap，将清除之前的内容
    showImageFeatures(false);
    imageModified(true);
}


void MainWindow::on_actImg_ZoomOut_triggered()
{//缩小
    int W=m_image.width();
    int H=m_image.height();
    m_image=m_image.scaled(0.9*W, 0.9*H,Qt::KeepAspectRatio);   //缩小
    QPixmap  pixmap=QPixmap::fromImage(m_image);
    ui->labPic->setPixmap(pixmap);      //刷新界面的图像显示
    showImageFeatures(false);
    imageModified(true);
}

