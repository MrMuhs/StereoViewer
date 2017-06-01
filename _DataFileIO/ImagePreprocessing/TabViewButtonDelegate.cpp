// StereoViewer is a volume rendering program with a stereoscopic view
// Copyright (C) 2009 Pascal Mues
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "TabViewButtonDelegate.h"

TabViewButtonDelegate::TabViewButtonDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *TabViewButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    TabViewEditButton *editor = new TabViewEditButton(parent);
    editor->setText("Einstellung");
    QString OperationName = index.model()->data(index).toString();
    if(OperationName == "Medianfilter")
    {
        editor->m_Operation = new MedianFilter(0);
    }
    if(OperationName == "Histogramm")
    {
        editor->m_Operation = new HistogramGenerator(0);
    }
    if(OperationName == "Unscharfmaskieren")
    {
        editor->m_Operation = new UnsharpMasking(0);
    }
    if(editor->m_Operation != 0)
        QObject::connect(editor, SIGNAL(clicked()), editor->m_Operation, SLOT(m_vIfUserWantsToChangeSettings()));
    return editor;
}

void TabViewButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//    int value = index.model()->data(index, Qt::EditRole).toInt();

//    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//    spinBox->setValue(value);
    //QVariant sadf = index.model()->data(index, Qt::EditRole);
}

void TabViewButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//    spinBox->interpretText();
//    int value = spinBox->value();

//    model->setData(index, value, Qt::EditRole);
}

void TabViewButtonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
