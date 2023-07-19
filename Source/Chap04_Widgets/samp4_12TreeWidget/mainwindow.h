#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include    <QMainWindow>
#include    <QTreeWidgetItem>
#include    <QLabel>
#include    <QSpinBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
//枚举类型treeItemType， 用于创建 QTreeWidgetItem 时作为节点的type, 自定义类型必须大于1000
//itTopItem 顶层节点;  itGroupItem 组节点； itImageItem 图片
    enum    treeItemType{itTopItem=1001,itGroupItem,itImageItem};

//枚举类型，表示列号
    enum    treeColNum{colItem=0, colItemType=1, colDate}; //目录树列的编号定义

    QLabel  *labFileName;
    QLabel  *labNodeText;
    QSpinBox *spinRatio;

    QPixmap m_pixmap; //当前的图片
    float   m_ratio;//当前图片缩放比例

    void    buildTreeHeader();  //构建表头
    void    iniTree();//目录树初始化

    void    addFolderItem(QTreeWidgetItem *parItem, QString dirName);//添加一个目录节点

    QString getFinalFolderName(const QString &fullPathName);//从目录全名称中获取最后的文件夹名称

    void    addImageItem(QTreeWidgetItem *parItem,QString aFilename);//添加一个图片节点

    void    displayImage(QTreeWidgetItem *item); //显示一个图片节点的图片

    void    changeItemCaption(QTreeWidgetItem *item); //遍历改变节点标题

    void    deleteItem(QTreeWidgetItem *parItem, QTreeWidgetItem *item);  //彻底删除一个节点，及其子节点

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

//目录树当前节点变动的信号
    void on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actAddFolder_triggered();//添加目录节点

    void on_actAddFiles_triggered();//添加图片节点

    void on_actZoomOut_triggered(); //缩小，zoom out

    void on_actZoomIn_triggered(); //放大，zoom in

    void on_actZoomFitW_triggered(); //适合宽度

    void on_actZoomFitH_triggered();//适合高度

    void on_actZoomRealSize_triggered(); //实际大小

    void on_actDeleteItem_triggered(); //删除节点

    void on_actScanItems_triggered(); //遍历节点

    void on_dockWidget_visibilityChanged(bool visible);

    void on_dockWidget_topLevelChanged(bool topLevel);

    void on_actDockFloat_triggered(bool checked);

    void on_treeFiles_itemChanged(QTreeWidgetItem *item, int column);

    void on_treeFiles_itemCollapsed(QTreeWidgetItem *item);

    void on_treeFiles_itemExpanded(QTreeWidgetItem *item);

    void on_treeFiles_itemSelectionChanged();

    void on_treeFiles_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeFiles_itemPressed(QTreeWidgetItem *item, int column);


    void on_actDockVisible_triggered(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
