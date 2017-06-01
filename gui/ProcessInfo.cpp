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

#include "ProcessInfo.h"
#include "ui_ProcessInfo.h"

ProcessInfo::ProcessInfo(QWidget *parent, QString stitle) :
    QWidget(parent),
    ui(new Ui::ProcessInfo)
{
    ui->setupUi(this);
    m_sMessage = "Processing ...";
    ui->lblMessage->setText(m_sMessage);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    this->setWindowTitle(stitle);
    m_SecondsCountUpTimer = new QTimer();
    QObject::connect(m_SecondsCountUpTimer, SIGNAL(timeout()), this, SLOT(m_vTimerUpdate()));
    QObject::connect(this->ui->pushButton, SIGNAL(clicked()), this, SLOT(m_vUserClickAbortIntern()));
}

ProcessInfo::~ProcessInfo()
{
    delete m_SecondsCountUpTimer;
    delete ui;
}

void ProcessInfo::closeEvent(QCloseEvent *event)
{
    m_SecondsCountUpTimer->stop();
    emit m_vUserClickAbort();
}

void ProcessInfo::m_vProcessChanged(QString msg, int percentage)
{
    if(percentage >= 0 && percentage <= 100)
    {
        ui->progressBar->setValue(percentage);
        m_sMessage = msg;
        if(percentage == 0)
        {
            m_iSecondsSinceZeroPercent = 0;
            ui->lblMessage->setText(m_sMessage + " (k.A.)");
            m_SecondsCountUpTimer->start(1000);
        }
    }
}

void ProcessInfo::m_vUserClickAbortIntern()
{
    this->ui->pushButton->setEnabled(false);
    m_SecondsCountUpTimer->stop();
    ui->lblMessage->setText("Abbrechen ...");
    emit m_vUserClickAbort();
}

void ProcessInfo::m_vTimerUpdate()
{
    int percentage = ui->progressBar->value();
    m_iSecondsSinceZeroPercent++;
    if(percentage > 0)
    {
        double days = (100-percentage) * ((double)m_iSecondsSinceZeroPercent / (double)percentage);
        if(days < 100)
           ui->lblMessage->setText(m_sMessage + " (" + QString::number((int)days) + " Sekunden)");
        else
           ui->lblMessage->setText(m_sMessage + " (" + QString::number((int)(days/60.0)) + " Minuten)");
    }
    else
    {
        ui->lblMessage->setText(m_sMessage + " (k.A.)");
    }
}
