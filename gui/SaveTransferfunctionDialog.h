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

#ifndef SAVETRANSFERFUNCTIONDIALOG_H
#define SAVETRANSFERFUNCTIONDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
    class SaveTransferfunctionDialog;
}

class SaveTransferfunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveTransferfunctionDialog(QWidget *parent = 0);
    ~SaveTransferfunctionDialog();
    void m_vSetPreValues(QString filename, QString description, QString last);
    QString m_sFilename;
    QString m_sDescription;
    QString m_sLastPath;

private slots:
    void m_vSelectFile();
    void m_vCheckOkay();
    void m_vButtonCancel();

private:
    Ui::SaveTransferfunctionDialog *ui;
};

#endif // SAVETRANSFERFUNCTIONDIALOG_H
