/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TMYDEVICE_H
#define TMYDEVICE_H

#include  <QtCharts>
#include  <QIODevice>

class TMyDevice : public QIODevice
{
    Q_OBJECT

private:
    qint64  m_range=4000;       //图表序列的最多数据点个数
    bool    m_drawChart=true;   //是否要绘制曲线
    QLineSeries *m_series;      //图表序列

    bool    m_saveToFile=false; //是否要保存到文件
    QString m_fileName;     //保存的文件名
    QFile   m_file;         //QFile对象
public:
    TMyDevice(QObject *parent = nullptr);

    void    openDAQ(qint64 pointsCount, bool drawChart, QLineSeries *series,
                    bool saveToFile, QString filename);  //代替open()函数
    void    closeDAQ(); //代替close()函数
protected:
    qint64 readData(char * data, qint64 maxSize);       //从设备读取数据到缓冲区
    qint64 writeData(const char * data, qint64 maxSize);//缓冲区数据写入设备
signals:
    void  updateBlockSize(qint64 blockSize);    //在writeData()里发射的一个信号
};

#endif // TMYDEVICE_H
