#include    "tfloatspindelegate.h"
#include    <QDoubleSpinBox>

TFloatSpinDelegate::TFloatSpinDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget *TFloatSpinDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(20000);
    editor->setDecimals(2);     //显示两位小数

    return editor;
}

void TFloatSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    float value = index.model()->data(index, Qt::EditRole).toFloat();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void TFloatSpinDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    //    spinBox->interpretText();
    float value = spinBox->value();
    QString str=QString::asprintf("%.2f",value);
    model->setData(index, str, Qt::EditRole);   //保存到数据模型
}

void TFloatSpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
