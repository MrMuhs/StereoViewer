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

#include "TabViewCheckboxDelegate.h"

TabViewCheckboxDelegate::TabViewCheckboxDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *TabViewCheckboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    QCheckBox* editor = new QCheckBox(parent);
    return editor;
}

void TabViewCheckboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::BackgroundRole).toBool();
    QCheckBox *spinBox = static_cast<QCheckBox*>(editor);
    spinBox->setChecked(value);
}

void TabViewCheckboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *spinBox = static_cast<QCheckBox*>(editor);
    bool value = spinBox->isChecked();
    model->setData(index, value, Qt::BackgroundRole);
}

void TabViewCheckboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
