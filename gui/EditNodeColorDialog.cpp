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

#include "EditNodeColorDialog.h"
#include "ui_EditNodeColorDialog.h"

EditNodeColorDialog::EditNodeColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditNodeColorDialog)
{
    ui->setupUi(this);
    m_ucGray = 0;
    m_ucRed = 0;
    m_ucGreen = 0;
    m_ucBlue = 0;
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(m_vCheckOkay()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(m_vSelectColor()));
    QObject::connect(ui->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(m_vColorRedChanged()));
    QObject::connect(ui->spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(m_vColorGreenChanged()));
    QObject::connect(ui->spinBox_4, SIGNAL(valueChanged(int)), this, SLOT(m_vColorBlueChanged()));
}

EditNodeColorDialog::~EditNodeColorDialog()
{
    delete ui;
}

void EditNodeColorDialog::m_vSetPreValues(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue)
{
    ui->spinBox->setValue((int)gray);
    ui->spinBox_2->setValue((int)red);
    ui->spinBox_3->setValue((int)green);
    ui->spinBox_4->setValue((int)blue);
    m_vSetResColor(red, green, blue);
}

void EditNodeColorDialog::m_vCheckOkay()
{
    m_ucGray = ui->spinBox->value();
    m_ucRed = ui->spinBox_2->value();
    m_ucGreen = ui->spinBox_3->value();
    m_ucBlue = ui->spinBox_4->value();

    this->accept();
}

void EditNodeColorDialog::m_vSelectColor()
{
    QColorDialog* coldia = new QColorDialog(QColor(m_ucRed, m_ucGreen, m_ucBlue));
    if(coldia->exec()  == QDialog::Accepted)
    {
        m_ucRed = coldia->currentColor().red();
        m_ucGreen = coldia->currentColor().green();
        m_ucBlue = coldia->currentColor().blue();

        ui->spinBox_2->setValue((int)m_ucRed);
        ui->spinBox_3->setValue((int)m_ucGreen);
        ui->spinBox_4->setValue((int)m_ucBlue);

        m_vSetResColor(m_ucRed, m_ucGreen, m_ucBlue);
    }
}

void EditNodeColorDialog::m_vColorRedChanged()
{
    m_ucRed = ui->spinBox_2->value();
    m_vSetResColor(m_ucRed, m_ucGreen, m_ucBlue);
}

void EditNodeColorDialog::m_vColorGreenChanged()
{
    m_ucGreen = ui->spinBox_3->value();
    m_vSetResColor(m_ucRed, m_ucGreen, m_ucBlue);
}

void EditNodeColorDialog::m_vColorBlueChanged()
{
    m_ucBlue = ui->spinBox_4->value();
    m_vSetResColor(m_ucRed, m_ucGreen, m_ucBlue);
}

void EditNodeColorDialog::m_vSetResColor(unsigned char red, unsigned char green, unsigned char blue)
{
    QPalette palette = ui->frame->palette();
    palette.setColor(backgroundRole(), QColor(red, green, blue));
    ui->frame->setPalette(palette);
    ui->frame->setAutoFillBackground(true);
}
