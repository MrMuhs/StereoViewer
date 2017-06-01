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

#include "MedianFilterDialog.h"
#include "ui_MedianFilterDialog.h"

MedianFilterDialog::MedianFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianFilterDialog)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    ui->setupUi(this);
    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vButtonOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
}

MedianFilterDialog::~MedianFilterDialog()
{
    delete ui;
}

void MedianFilterDialog::m_vButtonOkay()
{
    int input = ui->uisbBlocksize->value();
    if(input%2 == 0)
    {
        QMessageBox::critical(0, "Fehler", "Blockgröße: Ungerader Wert erforderlich");
        return;
    }
    m_iBlockSize = input;
    accept();
}

void MedianFilterDialog::m_vButtonCancel()
{
    reject();
}

bool MedianFilterDialog::m_bSetBlockSize(int size)
{
    if(size > ui->uisbBlocksize->maximum() || size < ui->uisbBlocksize->minimum())
    {
        m_LogFile->m_bAppendMessage(Error, "(MedianFilterDialog::m_bSetBlockSize(int)) Size out of range");
        return false;
    }
    else
    {
        m_iBlockSize = size;
        ui->uisbBlocksize->setValue(m_iBlockSize);
        return true;
    }
}

int MedianFilterDialog::m_iGetBlockSize()
{
    return m_iBlockSize;
}
