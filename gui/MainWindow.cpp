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

#include "gui/MainWindow.h"
#include <ui_MainWindow.h>

const QString MainWindow::cm_sNoVolumeWidget =  QString("<Keins vorhanden>");
const QString MainWindow::cm_sNoMultiWidget =  QString("<Keins vorhanden>");
const QString MainWindow::cm_sNoMultiWidgetSelected =  QString("<Nicht zugeordnet>");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_GlobalSettings = GlobalSettings::m_GlobalSettingsGetInstanz();

    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Programstart");
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Logfile opened");
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) BEGIN MainWindow(..)");

    // Setup the UI
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Userinterface initialise");
    ui->setupUi(this);
    ui->uicobStereoVerfahren->addItem("Anaglyph");
    ui->uicobStereoVerfahren->addItem("Polarisation Links");
    ui->uicobStereoVerfahren->addItem("Polarisation Rechts");
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Userinterface initialise .. done");

    // Create new Instance form Volume-Class
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Creating Volume");
    m_Volume = new Volume;
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Creating Volume .. done");

    // Setup Link to Transferfunction
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Setup Transferfunction");
    m_sLastTFLoadPath = "";
    m_sLastTFSavePath = "";
    ui->uiTFAlpha->m_bSetVolume(m_Volume);
    ui->uiTFColor->m_bSetVolume(m_Volume);
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Setup Transferfunction .. done");

    // Create new VolumeWidget
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Creating VolumeWidget");
    QGLFormat x = QGLFormat();
    if(x.hasOpenGL())
    {
        m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) OpenGL is available on this system, opening first VolumeWidget");
        m_iVolumeWidgetCounter = 0;
        m_iMultiWidgetCounter = 0;
        m_bChangeComesFromLoading = true;
        ui->uicobMultiWidgets->addItem(cm_sNoMultiWidget, 0);
        ui->uicobMultiWidgetForWidget->addItem(cm_sNoMultiWidgetSelected, 0);
        m_vEditStateMultiWidgetStettings(cm_sNoMultiWidget); // call slot manuell, not connected yet!
        m_bChangeComesFromLoading = false;
        m_bAddVolumeWidget();
        m_vIfVolWidgetCbIndexChanged(0);
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(MainWindow::MainWindow) OpenGL is not supported by this system(unavailable OpenGL Version)");
        m_vEditStateMultiWidgetStettings(cm_sNoMultiWidget); // call slot manuell, not connected yet!
        m_vEditStateVolumeWidgetStettings(cm_sNoVolumeWidget); // call slot manuell, not connected yet!
        ui->uibtnAddMultiWidget->setEnabled(false);
        ui->uibtnAddVolumeWidget->setEnabled(false);
        QMessageBox::critical(this, "StereoViewer",
                                    "OpenGL is not supported by this system, can't open a VolumeWidget");
    }
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Creating VolumeWidget .. done");

    // Connections Erstellen
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Setup Connections");
    // Menuebar
    QObject::connect(ui->actionBilderstack_ffnen, SIGNAL(triggered()), this, SLOT(m_vOpenImgStack()));
    QObject::connect(ui->actionVolumefile_ffnen, SIGNAL(triggered()), this, SLOT(m_vOpenVGIFile()));
    QObject::connect(ui->actionInfo, SIGNAL(triggered()), this, SLOT(m_vOpenInfo()));
    QObject::connect(ui->actionEinstellungen, SIGNAL(triggered()), this, SLOT(m_vSettingsDialog()));
    QObject::connect(ui->actionBeenden, SIGNAL(triggered()), this, SLOT(m_vClose()));
    // Widget management
    QObject::connect(ui->uibtnAddVolumeWidget, SIGNAL(clicked()), this, SLOT(m_vAddVolumeWidgetSlot()));
    QObject::connect(ui->uibtnRemoveVolumeWidget, SIGNAL(clicked()), this, SLOT(m_vRemoveVolumeWidgetSlot()));
    QObject::connect(ui->uicobVolumeWidgets, SIGNAL(currentIndexChanged(int)), this, SLOT(m_vIfVolWidgetCbIndexChanged(int)));
    QObject::connect(ui->uicobVolumeWidgets, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_vEditStateVolumeWidgetStettings(QString)));
    QObject::connect(ui->uibtnAddMultiWidget, SIGNAL(clicked()), this, SLOT(m_vAddMultiWidgetSlot()));
    QObject::connect(ui->uibtnRemoveMultiWidget, SIGNAL(clicked()), this, SLOT(m_vRemoveMultiWidgetSlot()));
    QObject::connect(ui->uicobMultiWidgetForWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(m_vIfMultiWidgetForWidgetCbIndexChanged(int)));
    QObject::connect(ui->uickbToggleFullScreen, SIGNAL(toggled(bool)), this, SLOT(m_vIfFullScreenIsToggled(bool)));
    QObject::connect(ui->uicobMultiWidgets, SIGNAL(currentIndexChanged(int)), this, SLOT(m_vIfMultiWidgetsIndexChanged(int)));
    QObject::connect(ui->uicobMultiWidgets, SIGNAL(currentIndexChanged(QString)), this, SLOT(m_vEditStateMultiWidgetStettings(QString)));
    // Transferfunction
    QObject::connect(ui->uibtnTFOeffnen, SIGNAL(clicked()), this, SLOT(m_vOpenTransferfunction()));
    QObject::connect(ui->uibtnTFSpeichern, SIGNAL(clicked()), this, SLOT(m_vSaveTransferfunction()));
    QObject::connect(ui->uicobHistogram, SIGNAL(currentIndexChanged(int)), ui->uiTFAlpha, SLOT(m_vSlotHistogramScalingChanged(int)));
    // Camerasettings
    QObject::connect(ui->uidsbClipNear, SIGNAL(valueChanged(double)), this, SLOT(m_vSyncCameraSettings()));
    QObject::connect(ui->uidsbClipFar, SIGNAL(valueChanged(double)), this, SLOT(m_vSyncCameraSettings()));
    QObject::connect(ui->uidsbFieldOfView, SIGNAL(valueChanged(double)), this, SLOT(m_vSyncCameraSettings()));
    QObject::connect(ui->uickbStereoAn, SIGNAL(toggled(bool)), this, SLOT(m_vSyncStereoSettings()));
    QObject::connect(ui->uicobStereoVerfahren, SIGNAL(currentIndexChanged(int)), this, SLOT(m_vSyncStereoSettings()));
    QObject::connect(ui->uidsbStereobasis, SIGNAL(valueChanged(double)), this, SLOT(m_vSyncStereoSettings()));
    QObject::connect(ui->uidsbFernpunktweite, SIGNAL(valueChanged(double)), this, SLOT(m_vSyncStereoSettings()));
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) Setup Connections .. done");
    m_LogFile->m_bAppendMessage(Load, "(MainWindow::MainWindow) END MainWindow(..)");
}

MainWindow::~MainWindow()
{
    m_LogFile->m_bAppendMessage(Debug, "(MainWindow::~MainWindow) Executing");
    delete ui;
    if(m_Volume) delete m_Volume;
    m_LogFile->m_bAppendMessage(Debug, "(MainWindow::~MainWindow) OK");
    //dumpObjectInfo();
}

void MainWindow::m_vOpenImgStack()
{
    OpenImgStackDialog m_OpenImgStackDiolog;
    if(m_OpenImgStackDiolog.exec() == QDialog::Accepted)
    {
        m_DataLoader = new DataLoader(m_OpenImgStackDiolog.m_ImgStack);
        QObject::connect(m_DataLoader, SIGNAL(finished()), this, SLOT(m_vIfDataLoaderFinishedLoading()));
        QObject::connect(m_DataLoader, SIGNAL(terminated()), this, SLOT(m_vIfDataLoaderWasTerminated()));
        m_DataLoader->m_vStartLoading();
    }
}

void MainWindow::m_vOpenVGIFile()
{
    OpenVgiVolFilesDialog m_Dialog;
    if(m_Dialog.exec() == QDialog::Accepted)
    {
        m_DataLoader = new DataLoader(m_Dialog.m_RawFile);
        QObject::connect(m_DataLoader, SIGNAL(finished()), this, SLOT(m_vIfDataLoaderFinishedLoading()));
        QObject::connect(m_DataLoader, SIGNAL(terminated()), this, SLOT(m_vIfDataLoaderWasTerminated()));
        m_DataLoader->m_vStartLoading();
    }
}

void MainWindow::m_vOpenInfo()
{
    Infomation dia;
    dia.exec();
}

void MainWindow::m_vOpenTransferfunction()
{
    QString last;
    if(m_sLastTFLoadPath == "")
        last = QDir::currentPath();
    else
        last = m_sLastTFLoadPath;

    QString sFile = QFileDialog::getOpenFileName(this, "Datei öffnen", last, "XML-Dateien (*.xml)");

    if (!sFile.isEmpty())
    {
        QString sFileReverse = strreverse(sFile);
        m_sLastTFLoadPath = strreverse(sFileReverse.remove(0, sFileReverse.indexOf("/")));

        if(m_Volume->m_GetTransferfuction()->m_bGenerateTFFromXMLFile(sFile))
        {
            ui->uiTFAlpha->m_vRepaintSceneContent();
            ui->uiTFColor->m_vRepaintSceneContent();
        }
    }
}

void MainWindow::m_vSaveTransferfunction()
{
    QString last;
    if(m_sLastTFLoadPath == "")
        last = QDir::currentPath();
    else
        last = m_sLastTFSavePath;

    SaveTransferfunctionDialog dia;
    dia.m_vSetPreValues(m_Volume->m_GetTransferfuction()->m_sGetFilename(), m_Volume->m_GetTransferfuction()->m_sGetDescription(), last);
    if(dia.exec() == QDialog::Accepted)
    {
        QString sFileReverse = strreverse(dia.m_sFilename);
        m_sLastTFSavePath = strreverse(sFileReverse.remove(0, sFileReverse.indexOf("/")));

        m_Volume->m_GetTransferfuction()->m_bGenerateTFToXMLFile(dia.m_sFilename, dia.m_sDescription);
    }
}

void MainWindow::m_vSettingsDialog()
{
    SettingsDialog dia;
    dia.exec();
}

void MainWindow::m_vClose()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    for(int i=0; i<m_VolumeWidgets.count(); i++)
    {
        m_VolumeWidgets[i]->close();
    }
    for(int i=0; i<m_MultiWidgets.count(); i++)
    {
        m_MultiWidgets[i]->close();
    }
 }

bool MainWindow::m_bAddVolumeWidget()
{
    m_VolumeWidgets.append(new VolumeWidget());
    int index = m_VolumeWidgets.count()-1;
    m_VolumeWidgets[index]->m_bSetVolume(m_Volume);
    m_VolumeWidgets[index]->setWindowTitle("VolumeView " + QString::number(++m_iVolumeWidgetCounter));
    m_VolumeWidgets[index]->show();
    QObject::connect(m_VolumeWidgets[index], SIGNAL(m_bIamClosing(QString)), this, SLOT(m_vInternCloseVolumeWidget(QString)));
    QObject::connect(m_VolumeWidgets[index], SIGNAL(m_vMyOpenGLChangedThePolarCam()), this, SLOT(m_vIfPolarWidgetChangedTheGlobalPolarCamera()));
    QObject::connect(m_Volume, SIGNAL(m_vVolumeDataChanged()), m_VolumeWidgets[index], SLOT(m_vIfDataChanged()));
    QObject::connect(this, SIGNAL(m_vCameraSettingsChanged()), m_VolumeWidgets[index], SLOT(m_vIfCameraSettingsChanged()));
    QObject::connect(this, SIGNAL(m_vPolarCameraChanged()), m_VolumeWidgets[index], SLOT(m_vIfAnotherWidgetChangedThePolarCam()));
    QObject::connect(ui->uiTFAlpha, SIGNAL(m_vTransferfunctionHasChanged()), m_VolumeWidgets[index], SLOT(m_vIfTransferfunctionChanged()));
    QObject::connect(ui->uiTFColor, SIGNAL(m_vTransferfunctionHasChanged()), m_VolumeWidgets[index], SLOT(m_vIfTransferfunctionChanged()));
    m_vFillComboBoxWithVolWidgets();
    return true;
}

bool MainWindow::m_bCloseVolumeWidget(QString sTitel)
{
    int index = ui->uicobVolumeWidgets->findText(sTitel);
    if(index == -1) return false;
    m_VolumeWidgets.removeAt(index);

    m_vFillComboBoxWithVolWidgets();
    return true;
}

void MainWindow::m_vFillComboBoxWithVolWidgets()
{
    m_bChangeComesFromLoading = true;
    ui->uicobVolumeWidgets->clear();
    if(m_VolumeWidgets.count() == 0)
    {
        ui->uicobVolumeWidgets->addItem(cm_sNoVolumeWidget, 0);
    }
    else
    {
        for(int i=0; i<m_VolumeWidgets.count(); i++)
        {
            ui->uicobVolumeWidgets->addItem(m_VolumeWidgets[i]->windowTitle(), i);
        }
    }
    m_bChangeComesFromLoading = false;
}

void MainWindow::m_vAddVolumeWidgetSlot()
{
    m_bAddVolumeWidget();
}

void MainWindow::m_vRemoveVolumeWidgetSlot()
{
    if(ui->uicobVolumeWidgets->count() > 0)
    {
        int index = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();
        // the volumewidget fires his intern closing event, so widgets will be close just one way, dont care intern/extern
        m_VolumeWidgets[index]->close();
    }
}

void MainWindow::m_vInternCloseVolumeWidget(QString ClosingTitel)
{
    m_bCloseVolumeWidget(ClosingTitel);
}

void MainWindow::m_vIfVolWidgetCbIndexChanged(int cbindex)
{
    if (m_bChangeComesFromLoading == true) return;
    if(ui->uicobVolumeWidgets->count() > 0)
    {
        m_bChangeComesFromLoading = true; // dont handle change signals!
        int index = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();

        if(m_VolumeWidgets[index]->m_sGetMultiWidgetTitle() == "")
        {
            ui->uicobMultiWidgetForWidget->setCurrentIndex(0);
        }
        else
        {
            int indexcbmulti = ui->uicobMultiWidgetForWidget->findText(m_VolumeWidgets[index]->m_sGetMultiWidgetTitle());
            ui->uicobMultiWidgetForWidget->setCurrentIndex(indexcbmulti);
        }

        ui->uidsbClipNear->setValue(m_VolumeWidgets[index]->m_GetRefCamera()->m_dGetNear());
        ui->uidsbClipFar->setValue(m_VolumeWidgets[index]->m_GetRefCamera()->m_dGetFar());
        ui->uidsbFieldOfView->setValue(m_VolumeWidgets[index]->m_GetRefCamera()->m_dGetFieldOfView());

        ui->uidsbStereobasis->setValue(m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_dStereoBasis);
        ui->uidsbFernpunktweite->setValue(m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_dFernpunktweite);
        ui->uickbStereoAn->setChecked(m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_bStereoOn);
        switch(m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_StereoMethod)
        {
            case Anaglyp: ui->uicobStereoVerfahren->setCurrentIndex(0); break;
            case PolarisationLeft: ui->uicobStereoVerfahren->setCurrentIndex(1); break;
            case PolarisationRight: ui->uicobStereoVerfahren->setCurrentIndex(2); break;
        }
        m_bChangeComesFromLoading = false;
    }
}

// #####################################
// Funktions for MultiWidget Management ->
// #####################################

bool MainWindow::m_bAddMultiWidget()
{
    m_MultiWidgets.append(new MultiWidget());
    int index = m_MultiWidgets.count()-1;
    m_MultiWidgets[index]->setWindowTitle("Sammelfenster " + QString::number(++m_iMultiWidgetCounter));
    m_MultiWidgets[index]->show();
    QObject::connect(m_MultiWidgets[index], SIGNAL(m_bIamClosing(QString)), this, SLOT(m_vInternCloseMultiView(QString)));
    m_vFillComboBoxWithMultiWidgets();
    return true;
}

bool MainWindow::m_bCloseMultiWidget(QString sTitel)
{
    int index = ui->uicobMultiWidgets->findText(sTitel);
    if(index == -1) return false;
    m_MultiWidgets.removeAt(index);

    m_vFillComboBoxWithMultiWidgets();
    return true;
}

void MainWindow::m_vFillComboBoxWithMultiWidgets()
{
    m_bChangeComesFromLoading = true;
    ui->uicobMultiWidgets->clear();
    ui->uicobMultiWidgetForWidget->clear();
    ui->uicobMultiWidgetForWidget->addItem("<Nicht zugeordnet>", 0);
    if(m_MultiWidgets.count() == 0)
    {
        ui->uicobMultiWidgets->addItem(cm_sNoMultiWidget, 0);
    }
    else
    {
        for(int i=0; i<m_MultiWidgets.count(); i++)
        {
            ui->uicobMultiWidgets->addItem(m_MultiWidgets[i]->windowTitle(), i);
            ui->uicobMultiWidgetForWidget->addItem(m_MultiWidgets[i]->windowTitle(), i+1);
        }
    }
    m_bChangeComesFromLoading = false;
}

void MainWindow::m_vAddMultiWidgetSlot()
{
    m_bAddMultiWidget();
}

void MainWindow::m_vRemoveMultiWidgetSlot()
{
    if(ui->uicobMultiWidgets->count() >= 0  && ui->uicobMultiWidgets->currentText() != cm_sNoMultiWidget)
    {
        int index = ui->uicobMultiWidgets->itemData(ui->uicobMultiWidgets->currentIndex()).toInt();
        // the multiwidget fires his intern closing event, so widgets will be close just one way, dont care intern/extern
        QString oldtitel = m_MultiWidgets[index]->windowTitle();
        m_MultiWidgets[index]->close();
        foreach(VolumeWidget* volwid, m_VolumeWidgets)
        {
            if(volwid->m_sGetMultiWidgetTitle() == oldtitel)
            {
                volwid->m_bSetMultiWidgetTitle("");
                volwid->show();
            }
        }
        m_vFillComboBoxWithMultiWidgets();
    }
}

void MainWindow::m_vIfMultiWidgetsIndexChanged(int index)
{
    if(ui->uicobMultiWidgets->currentText() != cm_sNoMultiWidget && index >= 0)
    {
        m_bChangeComesFromLoading = true;
        if(m_MultiWidgets[index]->isFullScreen() == true)
            ui->uickbToggleFullScreen->setChecked(true);
        else
            ui->uickbToggleFullScreen->setChecked(false);
        m_bChangeComesFromLoading = false;
    }
}

void MainWindow::m_vIfMultiWidgetForWidgetCbIndexChanged(int index)
{
    if (m_bChangeComesFromLoading == true) return;
    if(index == 0)
    {
        // get the volumewidget out of the multiwidget
        int indexvolwid = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();
        int multiindex = ui->uicobMultiWidgets->findText(m_VolumeWidgets[indexvolwid]->m_sGetMultiWidgetTitle());
        if (multiindex >= 0)
        {
            m_MultiWidgets[multiindex]->m_bRemoveSubWidget(m_VolumeWidgets[indexvolwid]->windowTitle());
            m_VolumeWidgets[indexvolwid]->show();
        }
    }
    if(index > 0)
    {
        // setup the volumewidget selected volumewidget into a multiwidget
        int indexvolwid = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();

        if(m_VolumeWidgets[indexvolwid]->m_sGetMultiWidgetTitle() != "" &&
           m_MultiWidgets[index - 1]->windowTitle() != m_VolumeWidgets[indexvolwid]->m_sGetMultiWidgetTitle())
        {
            // the volume widget is into another multiwidget, remove it first
            int oldmultiwidindex = ui->uicobMultiWidgets->findText(m_VolumeWidgets[indexvolwid]->m_sGetMultiWidgetTitle());
            m_MultiWidgets[oldmultiwidindex]->m_bRemoveSubWidget(m_VolumeWidgets[indexvolwid]->windowTitle());
        }

        m_MultiWidgets[index - 1]->m_bAddSubWidget(m_VolumeWidgets[indexvolwid]);
    }
}

void MainWindow::m_vIfFullScreenIsToggled(bool stat)
{
    if(m_bChangeComesFromLoading == true) return;
    if(ui->uicobMultiWidgets->currentText() != cm_sNoMultiWidget)
    {
        int multiindex = ui->uicobMultiWidgets->currentIndex();
        if(m_MultiWidgets[multiindex]->isFullScreen() == true)
            m_MultiWidgets[multiindex]->showNormal();
        else
            m_MultiWidgets[multiindex]->showFullScreen();
    }
}

void MainWindow::m_vInternCloseMultiView(QString ClosingTitel)
{
    m_bCloseMultiWidget(ClosingTitel);
}

// #####################################
// <- Functions for MultiView Management
// #####################################

void MainWindow::m_vSyncCameraSettings()
{
    if (m_bChangeComesFromLoading == true) return;
    if(ui->uicobVolumeWidgets->count() > 0)
    {
        int index = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();
        m_VolumeWidgets[index]->m_GetRefCamera()->m_bSetNear(ui->uidsbClipNear->value());
        m_VolumeWidgets[index]->m_GetRefCamera()->m_bSetFar(ui->uidsbClipFar->value());
        m_VolumeWidgets[index]->m_GetRefCamera()->m_bSetFieldOfView(ui->uidsbFieldOfView->value());
        emit m_vCameraSettingsChanged();
    }
}

void MainWindow::m_vSyncStereoSettings()
{
    if (m_bChangeComesFromLoading == true) return;
    if(ui->uicobVolumeWidgets->count() > 0)
    {
        int index = ui->uicobVolumeWidgets->itemData(ui->uicobVolumeWidgets->currentIndex()).toInt();
        m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_bStereoOn = ui->uickbStereoAn->isChecked();
        switch(ui->uicobStereoVerfahren->currentIndex())
        {
            case 0: m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_StereoMethod = Anaglyp; break;
            case 1: m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_StereoMethod = PolarisationLeft; break;
            case 2: m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_StereoMethod = PolarisationRight; break;
        }
        m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_dStereoBasis = ui->uidsbStereobasis->value();
        m_VolumeWidgets[index]->m_GetRefStereoSettings()->m_dFernpunktweite = ui->uidsbFernpunktweite->value();
        emit m_vCameraSettingsChanged();
    }
}

void MainWindow::m_vIfPolarWidgetChangedTheGlobalPolarCamera()
{
    emit m_vPolarCameraChanged();
}

void MainWindow::m_vIfDataLoaderFinishedLoading()
{
    TexArray3D* result = m_DataLoader->m_GetLoaderResult();
    if(result != 0)
    {
        m_Volume->m_bSetDataFile(m_DataLoader->m_GetLoaderResult());
        QObject::disconnect(this, SLOT(m_vIfDataLoaderFinishedLoading()));
        QObject::disconnect(this, SLOT(m_vIfDataLoaderWasTerminated()));
        delete m_DataLoader;
    }
    else
    {
        m_LogFile->m_bAppendMessage(Debug, "(MainWindow::m_vIfDataLoaderFinishedLoading) Laden abgebrochen.");
        QObject::disconnect(this, SLOT(m_vIfDataLoaderFinishedLoading()));
        QObject::disconnect(this, SLOT(m_vIfDataLoaderWasTerminated()));
        delete m_DataLoader;
    }
}

void MainWindow::m_vIfDataLoaderWasTerminated()
{
    QObject::disconnect(this, SLOT(m_vIfDataLoaderFinishedLoading()));
    QObject::disconnect(this, SLOT(m_vIfDataLoaderWasTerminated()));
    delete m_DataLoader;
}

void MainWindow::m_vEditStateMultiWidgetStettings(QString text)
{
    bool stat = false;
    if(text != cm_sNoMultiWidget) stat = true;
    ui->uickbToggleFullScreen->setEnabled(stat);
    ui->uibtnRemoveMultiWidget->setEnabled(stat);
}

void MainWindow::m_vEditStateVolumeWidgetStettings(QString text)
{
    bool stat = false;
    if(text != cm_sNoVolumeWidget) stat = true;
    ui->uibtnRemoveVolumeWidget->setEnabled(stat);
    ui->uickbStereoAn->setEnabled(stat);
    ui->uicobMultiWidgetForWidget->setEnabled(stat);
    ui->uicobStereoVerfahren->setEnabled(stat);
    ui->uidsbClipFar->setEnabled(stat);
    ui->uidsbClipNear->setEnabled(stat);
    ui->uidsbFernpunktweite->setEnabled(stat);
    ui->uidsbFieldOfView->setEnabled(stat);
    ui->uidsbStereobasis->setEnabled(stat);
}
