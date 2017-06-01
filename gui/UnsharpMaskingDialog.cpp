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

#include "UnsharpMaskingDialog.h"
#include "ui_UnsharpMaskingDialog.h"

UnsharpMaskingDialog::UnsharpMaskingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnsharpMaskingDialog)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    ui->setupUi(this);
    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vButtonOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
}

UnsharpMaskingDialog::~UnsharpMaskingDialog()
{
    delete ui;
}

void UnsharpMaskingDialog::m_vButtonOkay()
{
    // the controls should limited the values, no check req.
    m_iRadius = ui->uisbRadius->value();
    m_dMenge = ui->uidsbMenge->value();
    m_dThreshold = ui->uidsbThreshold->value();
    accept();
}

void UnsharpMaskingDialog::m_vButtonCancel()
{
    reject();
}

bool UnsharpMaskingDialog::m_bSetRadius(int radius)
{
    if(radius > ui->uisbRadius->maximum() || radius < ui->uisbRadius->minimum())
    {
        m_LogFile->m_bAppendMessage(Error, "(UnsharpMaskingDialog::m_bSetRadius(int)) Radius out of range");
        return false;
    }
    else
    {
        m_iRadius = radius;
        ui->uisbRadius->setValue(m_iRadius);
        return true;
    }
}

bool UnsharpMaskingDialog::m_bSetMenge(double menge)
{
    if(menge > ui->uidsbMenge->maximum() || menge < ui->uidsbMenge->minimum())
    {
        m_LogFile->m_bAppendMessage(Error, "(UnsharpMaskingDialog::m_bSetMenge(double)) Gewichtung out of range");
        return false;
    }
    else
    {
        m_dMenge = menge;
        ui->uidsbMenge->setValue(m_dMenge);
        return true;
    }
}

bool UnsharpMaskingDialog::m_bSetThreshold(double threshold)
{
    if(threshold > ui->uidsbThreshold->maximum() || threshold < ui->uidsbThreshold->minimum())
    {
        m_LogFile->m_bAppendMessage(Error, "(UnsharpMaskingDialog::m_bSetThreshold(double)) Threshold out of range");
        return false;
    }
    else
    {
        m_dThreshold = threshold;
        ui->uidsbThreshold->setValue(m_dThreshold);
        return true;
    }
}
