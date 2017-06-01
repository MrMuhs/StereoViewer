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

#ifndef MEDIANFILTERDIALOG_H
#define MEDIANFILTERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "_Misc/LogFile.h"

namespace Ui {
    class MedianFilterDialog;
}

class MedianFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianFilterDialog(QWidget *parent = 0);
    ~MedianFilterDialog();
    bool m_bSetBlockSize(int size);
    int m_iGetBlockSize();

private slots:
    void m_vButtonOkay();
    void m_vButtonCancel();

private:
    LogFile* m_LogFile;
    Ui::MedianFilterDialog *ui;
    int m_iBlockSize;
};

#endif // MEDIANFILTERDIALOG_H
