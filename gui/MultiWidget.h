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

#ifndef MULTIWIDGET_H
#define MULTIWIDGET_H

#include <QWidget>
#include "gui/VolumeWidget.h"

namespace Ui {
    class MultiWidget;
}

class MultiWidget : public QWidget
{
    Q_OBJECT
public:
    MultiWidget(QWidget *parent = 0);
    ~MultiWidget();
    void closeEvent(QCloseEvent *event);
    bool m_bAddSubWidget(VolumeWidget* subwid);
    bool m_bRemoveSubWidget(QString subwidtitle);

    QList<VolumeWidget*> m_SubWidgetsList;

signals:
    void m_bIamClosing(QString MyTitel);

private:
    bool m_bSetupUserInterface();
    bool m_bSetupLayout();

    Ui::MultiWidget* ui;
    QHBoxLayout* m_HorizontalLayout;
};

#endif // MULTIWIDGET_H
