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

#include "TabViewTextReadOnlyDelegate.h"

TabViewTextReadOnlyDelegate::TabViewTextReadOnlyDelegate()
{

}

QWidget* TabViewTextReadOnlyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex &index) const
{
    // totaly stupid in qt, u cant just set a flag for readonly, u have to implement a stupid delegate?! wtf man
    return 0;
}
