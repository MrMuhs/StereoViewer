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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QGLFormat>
#include "_Misc/_Defines.h"
#include "_Misc/GlobalSettings.h"
#include "_Misc/LogFile.h"
#include "_Volume/Volume.h"
#include "_Volume/TransferFunction/TransferFunctionViewAlpha.h"
#include "_DataFileIO/DataLoader.h"
#include "gui/SettingsDialog.h"
#include "gui/DataPreProcessingDialog.h"
#include "gui/MultiWidget.h"
#include "gui/VolumeWidget.h"
#include "gui/Infomation.h"
#include "gui/OpenImgStackDialog.h"
#include "gui/OpenVgiVolFilesDialog.h"
#include "gui/SaveTransferfunctionDialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private slots:
    void m_vOpenImgStack();
    void m_vOpenVGIFile();
    void m_vOpenInfo();
    void m_vOpenTransferfunction();
    void m_vSaveTransferfunction();
    void m_vSettingsDialog();
    void m_vClose();

    // Slots for VolumeWidget Management
    void m_vInternCloseVolumeWidget(QString ClosingTitel);
    void m_vAddVolumeWidgetSlot();
    void m_vRemoveVolumeWidgetSlot();
    void m_vIfVolWidgetCbIndexChanged(int index);
    void m_vEditStateVolumeWidgetStettings(QString text);

    // Slots for MultiWidget Management
    void m_vInternCloseMultiView(QString ClosingTitel);
    void m_vAddMultiWidgetSlot();
    void m_vRemoveMultiWidgetSlot();
    void m_vIfMultiWidgetsIndexChanged(int index);
    void m_vIfMultiWidgetForWidgetCbIndexChanged(int index);
    void m_vIfFullScreenIsToggled(bool stat);
    void m_vEditStateMultiWidgetStettings(QString text);

    void m_vSyncCameraSettings();
    void m_vSyncStereoSettings();
    void m_vIfPolarWidgetChangedTheGlobalPolarCamera(); // recieved from the widget that changes the camera
    void m_vIfDataLoaderFinishedLoading();
    void m_vIfDataLoaderWasTerminated();

signals:
    void m_vVolumeDataChanged();
    void m_vCameraSettingsChanged();
    void m_vPolarCameraChanged(); // send to all other widgets, if they are polar used -> refresh

private:
    static const QString cm_sNoVolumeWidget;
    static const QString cm_sNoMultiWidget;
    static const QString cm_sNoMultiWidgetSelected;
    GlobalSettings* m_GlobalSettings;
    DataLoader* m_DataLoader;
    LogFile* m_LogFile;
    Volume* m_Volume;
    iCamera *m_Camera;
    StereoSettings* m_StereoSettings;
    QList<VolumeWidget*> m_VolumeWidgets;
    int m_iVolumeWidgetCounter;
    int m_iMultiWidgetCounter;
    Ui::MainWindow *ui;
    QString m_sLastTFLoadPath;
    QString m_sLastTFSavePath;
    QList<MultiWidget*> m_MultiWidgets;
    bool m_bChangeComesFromLoading;

    // VolumeWidget managment functions
    bool m_bAddVolumeWidget();
    bool m_bCloseVolumeWidget(QString sTitel);
    void m_vFillComboBoxWithVolWidgets();

    // MultiWidget Management functions
    bool m_bAddMultiWidget();
    bool m_bCloseMultiWidget(QString sTitel);
    void m_vFillComboBoxWithMultiWidgets();

    QString strreverse(QString in)
    {
        QString out;
        int len=in.length();
        for(int i=len-1, j=0; i>=0; i--, j++)
        {
            // exchange elements
            out[j]=in[i];
        }
        return out;
    }
};

#endif // MAINWINDOW_H
