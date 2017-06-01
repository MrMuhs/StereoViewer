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

#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include "_Volume/Volume.h"

class MultiWidget;

namespace Ui {
    class VolumeWidget;
}

class VolumeWidget : public QWidget
{
    Q_OBJECT
public:
    VolumeWidget(QWidget *parent = 0);
    ~VolumeWidget();
    void closeEvent(QCloseEvent *event);
    bool m_bSetVolume(Volume* ivol);
    bool m_bSetMultiWidgetTitle(QString upwidget);
    QString m_sGetMultiWidgetTitle();
    iCamera* m_GetRefCamera();
    StereoSettings* m_GetRefStereoSettings();

public slots:
    void m_vIfDataChanged();
    void m_vIfTransferfunctionChanged();
    void m_vIfCameraSettingsChanged();
    void m_vIfAnotherWidgetChangedThePolarCam();
    void m_vIfMyOpenGlWidgetChanedThePolarCam();

signals:
    void m_bIamClosing(QString MyTitel);
    void m_vMyOpenGLChangedThePolarCam();

private:
    LogFile* m_LogFile;
    Ui::VolumeWidget *ui;
    QString m_ParentMultiWidgetTitle;
    QPoint oldpos;
    QSize oldsize;
};

#endif // VOLUMEWIDGET_H
