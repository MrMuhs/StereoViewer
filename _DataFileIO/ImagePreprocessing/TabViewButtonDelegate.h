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

#ifndef TABVIEWBUTTONDELEGATE_H
#define TABVIEWBUTTONDELEGATE_H

#include <QItemDelegate>
#include "_DataFileIO/ImagePreprocessing/TabViewEditButton.h"
#include "_DataFileIO/ImagePreprocessing/MedianFilter.h"
#include "_DataFileIO/ImagePreprocessing/HistogramGenerator.h"
#include "_DataFileIO/ImagePreprocessing/UnsharpMasking.h"

class TabViewButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TabViewButtonDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TABVIEWBUTTONDELEGATE_H
