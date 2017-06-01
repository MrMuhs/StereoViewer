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

#ifndef UNSHARPMASKINGDIALOG_H
#define UNSHARPMASKINGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "_Misc/LogFile.h"

namespace Ui {
class UnsharpMaskingDialog;
}

class UnsharpMaskingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UnsharpMaskingDialog(QWidget *parent = 0);
    ~UnsharpMaskingDialog();

    bool m_bSetRadius(int radius);
    bool m_bSetMenge(double menge);
    bool m_bSetThreshold(double threshold);
    int m_iGetRadius() { return m_iRadius; }
    double m_dGetMenge() { return m_dMenge; }
    double m_dGetThreshold() { return m_dThreshold; }
    
private slots:
    void m_vButtonOkay();
    void m_vButtonCancel();

private:
    LogFile* m_LogFile;
    Ui::UnsharpMaskingDialog *ui;
    int m_iRadius;
    double m_dMenge;
    double m_dThreshold;
};

#endif // UNSHARPMASKINGDIALOG_H
