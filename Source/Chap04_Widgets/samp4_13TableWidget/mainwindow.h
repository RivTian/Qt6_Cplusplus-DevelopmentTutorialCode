#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    //  自定义单元格Type的类型，在创建单元格的Item时使用
    enum    CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore}; //各单元格的类型

    //  各字段在表格中的列号
    enum    FieldColNum{colName=0, colSex,colBirth,colNation,colScore,colPartyM};

    QLabel  *labCellIndex; //状态栏上用于显示单元格的行号、列号
    QLabel  *labCellType;  //状态栏上用于显示单元格的type
    QLabel  *labStudID;    //状态栏上用于显示单元格的data,

    void    createItemsARow(int rowNo,QString name,QString sex,QDate birth,
                            QString nation,bool isPM,int score); //为某一行创建items
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetHeader_clicked();  //设置表头 按键

    void on_btnSetRows_clicked(); //设置行数 按键

    void on_btnIniData_clicked(); //初始化数据 按键

    void on_chkBoxTabEditable_clicked(bool checked); //表格可编辑 checkbox

    void on_chkBoxHeaderH_clicked(bool checked); //显示行表头 checkbox

    void on_chkBoxHeaderV_clicked(bool checked); //显示列表头 checkbox

    void on_chkBoxRowColor_clicked(bool checked); //间隔行底色 checkbox

    void on_rBtnSelectItem_clicked();   // 行选择模式 RadioButton

    void on_rBtnSelectRow_clicked();    // 单元格选择模式 RadioButton

    void on_btnReadToEdit_clicked(); //读取表格内容到文本 按键

    void on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnInsertRow_clicked();  //插入行 按键

    void on_btnAppendRow_clicked(); //添加行  按键

    void on_btnDelCurRow_clicked();     //删除当前行 按键

    void on_btnAutoHeght_clicked();

    void on_btnAutoWidth_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
