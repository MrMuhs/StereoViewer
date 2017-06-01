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

#ifndef OPENVGIVOLFILESDIALOG_H
#define OPENVGIVOLFILESDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "_Misc/LogFile.h"
#include "_DataFileIO/RawFile.h"
#include "gui/DataPreProcessingDialog.h"

namespace Ui {
    class OpenVgiVolFilesDialog;
}

class OpenVgiVolFilesDialog : public QDialog
{
    Q_OBJECT
public:
    OpenVgiVolFilesDialog(QWidget *parent = 0);
    ~OpenVgiVolFilesDialog();
    static iDataFile* m_RawFile;

protected:
    void changeEvent(QEvent *e);

private:
    LogFile* m_LogFile;
    Ui::OpenVgiVolFilesDialog *ui;

private slots:
    void m_vSelectFile();
    void m_vCheckFile();
    void m_vButtonOkay();
    void m_vButtonCancel();
};

#endif // OPENVGIVOLFILESDIALOG_H
