#ifndef TSPINBOXDELEGATE_H
#define TSPINBOXDELEGATE_H

#include <QStyledItemDelegate>

class TSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TSpinBoxDelegate(QObject *parent = nullptr);


    //自定义代理必须重新实现以下4个函数

    //创建编辑组件
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index)const;

    //从数据模型获取数据，显示到代理组件中
    void setEditorData(QWidget *editor, const QModelIndex &index)const;

    //将代理组件的数据，保存到数据模型中
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)const;

    //更新代理编辑组件的大小
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index)const;

};

#endif // TSPINBOXDELEGATE_H
