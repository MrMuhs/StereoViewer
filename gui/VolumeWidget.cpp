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

#include "gui/VolumeWidget.h"
#include <ui_VolumeWidget.h>

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeWidget)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    ui->setupUi(this);
    m_ParentMultiWidgetTitle = "";
    //showFullScreen();
    QObject::connect(ui->m_glWidget, SIGNAL(m_vIChangedThePolarCam()), this, SLOT(m_vIfMyOpenGlWidgetChanedThePolarCam()));
}

VolumeWidget::~VolumeWidget()
{
    delete ui;
}

void VolumeWidget::closeEvent(QCloseEvent *event)
{
    emit m_bIamClosing(windowTitle());
}

bool VolumeWidget::m_bSetVolume(Volume* ivol)
{
    ui->m_glWidget->m_Volume = ivol;
    return true;
}

bool VolumeWidget::m_bSetMultiWidgetTitle(QString upwidget)
{
    if(upwidget == "")
    {
        this->setParent(0);
        this->resize(oldsize.width(), oldsize.height());
        this->move(oldpos);
    }
    else
    {
        oldpos = this->pos();
        oldsize = this->size();
    }
    m_ParentMultiWidgetTitle = upwidget;
    return true;
}

QString VolumeWidget::m_sGetMultiWidgetTitle()
{
    return m_ParentMultiWidgetTitle;
}

void VolumeWidget::m_vIfDataChanged()
{
    //ui->m_glWidget->m_bRefreshTextures();
    ui->m_glWidget->m_bRefreshDataTex();
    ui->m_glWidget->m_bRefreshTransferfunctionTex();
    ui->m_glWidget->repaint();
}

void VolumeWidget::m_vIfTransferfunctionChanged()
{
    //ui->m_glWidget->m_bRefreshTextures();
    ui->m_glWidget->m_bRefreshTransferfunctionTex();
    ui->m_glWidget->repaint();
}

iCamera* VolumeWidget::m_GetRefCamera()
{
    return ui->m_glWidget->m_Camera;
}

StereoSettings* VolumeWidget::m_GetRefStereoSettings()
{
    return ui->m_glWidget->m_StereoSettings;
}

void VolumeWidget::m_vIfCameraSettingsChanged()
{
    ui->m_glWidget->m_bReCalcCamera();
    ui->m_glWidget->repaint();
}

void VolumeWidget::m_vIfAnotherWidgetChangedThePolarCam()
{
    // if wigdet is polarisation -> refresh it
    if(ui->m_glWidget->m_StereoSettings->m_bStereoOn == true &&
       (ui->m_glWidget->m_StereoSettings->m_StereoMethod == PolarisationLeft || ui->m_glWidget->m_StereoSettings->m_StereoMethod == PolarisationRight))
    {
        ui->m_glWidget->m_bReCalcCamera();
        ui->m_glWidget->repaint();
    }
}

void VolumeWidget::m_vIfMyOpenGlWidgetChanedThePolarCam()
{
    emit m_vMyOpenGLChangedThePolarCam();
}
