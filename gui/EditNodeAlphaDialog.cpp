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

#include "EditNodeAlphaDialog.h"
#include "ui_EditNodeAlphaDialog.h"

EditNodeAlphaDialog::EditNodeAlphaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNodeAlphaDialog)
{
    ui->setupUi(this);
    m_ucGray = 0;
    m_ucAlpha = 0;
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(m_vCheckOkay()));
}

EditNodeAlphaDialog::~EditNodeAlphaDialog()
{
    delete ui;
}

void EditNodeAlphaDialog::m_vSetPreValues(unsigned char gray, unsigned char alpha)
{
    ui->spinBox->setValue((int)gray);
    ui->spinBox_2->setValue((int)alpha);
}

void EditNodeAlphaDialog::m_vCheckOkay()
{
    m_ucGray = ui->spinBox->value();
    m_ucAlpha = ui->spinBox_2->value();
    this->accept();
}
