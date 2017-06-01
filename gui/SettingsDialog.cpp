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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    ui->setupUi(this);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::X11BypassWindowManagerHint);
    ui->ui_cboLogfileMode->addItem("Debug");
    ui->ui_cboLogfileMode->addItem("Warning");
    ui->ui_cboLogfileMode->addItem("Error");

    m_GlobalSettings = GlobalSettings::m_GlobalSettingsGetInstanz();
    m_bGetFromGlobalSettings();

    QObject::connect(ui->ui_btnDefaults, SIGNAL(clicked()), this, SLOT(m_vClickedDefaults()));
    QObject::connect(ui->ui_btnOkay, SIGNAL(clicked()), this, SLOT(m_vClickedOkay()));
    QObject::connect(ui->ui_btnSave, SIGNAL(clicked()), this, SLOT(m_vClickedSave()));
    QObject::connect(ui->ui_btnAbort, SIGNAL(clicked()), this, SLOT(m_vClickedAbort()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::m_vClickedDefaults()
{
    m_GlobalSettings->m_bSetToDefaults();
    m_bGetFromGlobalSettings();
}

void SettingsDialog::m_vClickedOkay()
{
    m_bSetToGlobalSettings();
    accept();
}

void SettingsDialog::m_vClickedSave()
{
    m_bSetToGlobalSettings();
    m_GlobalSettings->m_bSaveToXML();
    accept();
}

void SettingsDialog::m_vClickedAbort()
{
    this->reject();
}

bool SettingsDialog::m_bGetFromGlobalSettings()
{
    QString sLogMode = m_GlobalSettings->m_sGetField("LogFileMode");
    if(sLogMode.toInt() == Debug) ui->ui_cboLogfileMode->setCurrentIndex(0);
    if(sLogMode.toInt() == Warning) ui->ui_cboLogfileMode->setCurrentIndex(1);
    if(sLogMode.toInt() == Error) ui->ui_cboLogfileMode->setCurrentIndex(2);

    return true;
}

bool SettingsDialog::m_bSetToGlobalSettings()
{
    if(ui->ui_cboLogfileMode->currentIndex() == Debug) m_GlobalSettings->m_bSetField("LogFileMode", "0");
    if(ui->ui_cboLogfileMode->currentIndex() == Warning) m_GlobalSettings->m_bSetField("LogFileMode", "1");
    if(ui->ui_cboLogfileMode->currentIndex() == Error) m_GlobalSettings->m_bSetField("LogFileMode", "2");

    return true;
}
