#ifndef TCOMBOBOXDELEGATE_H
#define TCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class TComboBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

private:
    QStringList m_itemList; //选择列表
    bool    m_editable;     //是否可编辑

public:
	explicit TComboBoxDelegate(QObject *parent = nullptr);

	//自定义函数，初始化设置列表内容，是否可编辑
	void    setItems(QStringList items, bool editable);

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

#endif // TCOMBOBOXDELEGATE_H
