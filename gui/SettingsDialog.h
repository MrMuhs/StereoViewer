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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDebug>
#include "_Misc/LogFile.h"
#include "_Misc/GlobalSettings.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void m_vClickedDefaults();
    void m_vClickedOkay();
    void m_vClickedSave();
    void m_vClickedAbort();

private:
    bool m_bGetFromGlobalSettings();
    bool m_bSetToGlobalSettings();

    LogFile* m_LogFile;
    GlobalSettings* m_GlobalSettings;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
