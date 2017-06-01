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

#include "MultiWidget.h"
#include "ui_MultiWidget.h"

MultiWidget::MultiWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiWidget)
{
    ui->setupUi(this);
    m_bSetupLayout();
}

MultiWidget::~MultiWidget()
{
    delete ui;
}

void MultiWidget::closeEvent(QCloseEvent *event)
{
    emit m_bIamClosing(windowTitle());
}

bool MultiWidget::m_bAddSubWidget(VolumeWidget* subwid)
{
    m_SubWidgetsList.append(subwid);
    subwid->m_bSetMultiWidgetTitle(windowTitle());
    m_bSetupUserInterface();
    return true;
}

bool MultiWidget::m_bRemoveSubWidget(QString subwidtitle)
{
    int i;
    for(i=0; i < m_SubWidgetsList.count(); i++)
    {
        if(m_SubWidgetsList[i]->windowTitle() == subwidtitle)
            break;
    }
    if(m_SubWidgetsList.count() > 0 && i >= 0)
    {
        m_SubWidgetsList[i]->m_bSetMultiWidgetTitle("");
        m_SubWidgetsList.removeAt(i);
        m_bSetupUserInterface();
        return true;
    }
    return false;
}

bool MultiWidget::m_bSetupUserInterface()
{
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    delete m_HorizontalLayout;
    m_bSetupLayout();

    foreach(VolumeWidget* subwid, m_SubWidgetsList)
    {
        sizePolicy.setHeightForWidth(subwid->sizePolicy().hasHeightForWidth());
        subwid->setSizePolicy(sizePolicy);
        m_HorizontalLayout->addWidget(subwid);
    }
    return true;
}

bool MultiWidget::m_bSetupLayout()
{
    m_HorizontalLayout = new QHBoxLayout(this);
    m_HorizontalLayout->setSpacing(0);
    m_HorizontalLayout->setContentsMargins(0, 0, 0, 0);
    m_HorizontalLayout->setObjectName(QString::fromUtf8("m_HorizontalLayout"));
    return true;
}
