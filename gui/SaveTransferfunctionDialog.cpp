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

#include "SaveTransferfunctionDialog.h"
#include "ui_SaveTransferfunctionDialog.h"

SaveTransferfunctionDialog::SaveTransferfunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveTransferfunctionDialog)
{
    ui->setupUi(this);
    m_sFilename = "";
    m_sDescription = "";
    m_sLastPath = "";
    QObject::connect(ui->uiSelectFile, SIGNAL(clicked()), this, SLOT(m_vSelectFile()));
    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vCheckOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
}

SaveTransferfunctionDialog::~SaveTransferfunctionDialog()
{
    delete ui;
}

void SaveTransferfunctionDialog::m_vSetPreValues(QString filename, QString description, QString last)
{
    m_sLastPath = last;
    ui->uileFileName->setText(filename);
    ui->uiptxtDescription->document()->setPlainText(description);
}

void SaveTransferfunctionDialog::m_vCheckOkay()
{
    m_sFilename = ui->uileFileName->text();
    m_sDescription = ui->uiptxtDescription->document()->toPlainText();
    this->accept();
}

void SaveTransferfunctionDialog::m_vButtonCancel()
{
    this->reject();
}

void SaveTransferfunctionDialog::m_vSelectFile()
{
    QString last;
    if(m_sLastPath == "")
        last = QDir::currentPath();
    else
        last = m_sLastPath;

    QString sFile = QFileDialog::getSaveFileName(this, "Datei speichern", last, "XML-Dateien (*.xml)");
    if (!sFile.isEmpty())
    {
        ui->uileFileName->setText(sFile);
    }
}
