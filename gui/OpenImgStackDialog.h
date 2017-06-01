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

#ifndef OPENIMGSTACKDIALOG_H
#define OPENIMGSTACKDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "_Misc/LogFile.h"
#include "_DataFileIO/ImgStack.h"
#include "gui/DataPreProcessingDialog.h"

namespace Ui {
    class OpenImgStackDialog;
}

class OpenImgStackDialog : public QDialog
{
    Q_OBJECT
public:
    OpenImgStackDialog(QWidget *parent = 0);
    ~OpenImgStackDialog();
    static iDataFile* m_ImgStack;

protected:
    void changeEvent(QEvent *e);

private:
    LogFile* m_LogFile;
    Ui::OpenImgStackDialog *ui;
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

private slots:
    void m_vSelectFile();
    void m_vButtonOkay();
    void m_vButtonCancel();
};

#endif // OPENIMGSTACKDIALOG_H
