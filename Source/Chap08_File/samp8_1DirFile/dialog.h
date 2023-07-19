#ifndef DIALOG_H
#define DIALOG_H

#include    <QDialog>
#include    <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {class Dialog;}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

private:
    QFileSystemWatcher  fileWatcher;    //用于监视文件系统
    void    showBtnInfo(QObject *btn);  //显示按钮的标题和toolTip

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void do_directoryChanged(const QString &path);
    void do_fileChanged(const QString &path);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_51_clicked();

    void on_pushButton_54_clicked();

    void on_pushButton_53_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_55_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_59_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_60_clicked();

    void on_pushButton_61_clicked();

    void on_pushButton_62_clicked();

    void on_pushButton_63_clicked();

    void on_pushButton_64_clicked();

    void on_pushButton_65_clicked();

    void on_pushButton_66_clicked();

    void on_pushButton_67_clicked();

    void on_pushButton_68_clicked();

    void on_pushButton_69_clicked();

    void on_pushButton_70_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
