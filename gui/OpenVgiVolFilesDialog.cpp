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

#include "gui/OpenVgiVolFilesDialog.h"
#include "ui_OpenVgiVolFilesDialog.h"

iDataFile* OpenVgiVolFilesDialog::m_RawFile;

OpenVgiVolFilesDialog::OpenVgiVolFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenVgiVolFilesDialog)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    ui->setupUi(this);
    m_RawFile = 0;
    QObject::connect(ui->uiSelectFile, SIGNAL(clicked()), this, SLOT(m_vSelectFile()));
    QObject::connect(ui->uiCheckFile, SIGNAL(clicked()), this, SLOT(m_vCheckFile()));
    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vButtonOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
}

OpenVgiVolFilesDialog::~OpenVgiVolFilesDialog()
{
    delete ui;
}

void OpenVgiVolFilesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void OpenVgiVolFilesDialog::m_vSelectFile()
{
    QString sFile = QFileDialog::getOpenFileName(this, "Datei öffnen", QDir::currentPath(), "Dokumente (*.vgi)");
    if (!sFile.isEmpty())
    {
        ui->uileFile->setText(sFile);
        m_vCheckFile();
    }
}

void OpenVgiVolFilesDialog::m_vButtonOkay()
{
    QString sFilename;
    int iWidth, iHeight, iDepth;

    sFilename = ui->uileFileVolume->text();
    iWidth = ui->uisbWidth->value();
    iHeight = ui->uisbHeight->value();
    iDepth = ui->uisbDepth->value();

    //TODO: check inputs ;)

    m_RawFile = new RawFile(sFilename, iWidth, iHeight, iDepth, 1);

    DataPreProcessingDialog m_Dialog;
    m_Dialog.m_bSetBaseFile(m_RawFile);
    if(m_Dialog.exec() == QDialog::Accepted)
    {
        // get decorated object back!
        m_RawFile = m_Dialog.m_oGetResultFile();
    }

    accept();
}

void OpenVgiVolFilesDialog::m_vButtonCancel()
{
    this->m_RawFile = 0;
    this->reject();
}

void OpenVgiVolFilesDialog::m_vCheckFile()
{
    // this methode reads the vgi file and writes the values to the ui
    QString sFilename;
    sFilename = ui->uileFile->text();
    if(!sFilename.isEmpty())
    {
        QFile data(sFilename);
        if (data.open(QFile::ReadOnly))
        {
            QTextStream stream(&data);
            QString str = stream.readAll();
            if(!str.isEmpty())
            {
                QString linebreak = "";
                if(str.indexOf("\r\n") > 0) linebreak = "\r\n";
                if(str.indexOf("\n\r") > 0) linebreak = "\n\r";
                if(!linebreak.isEmpty())
                {
                    QStringList x = str.split(linebreak);
                    bool bSize = false;
                    for(int i=0; i<x.count(); i++)
                    {
                        str = x.at(i);
                        if(str.left(4) == "size")
                        {
                            // SplitBehavior { KeepEmptyParts, SkipEmptyParts };
                            QStringList y = str.split(" ", QString::SkipEmptyParts);
                            if(y.count() == 5)
                            {
                                str = y.at(2);
                                int width = str.toInt();
                                str = y.at(3);
                                int height = str.toInt();
                                str = y.at(4);
                                int depth = str.toInt();

                                ui->uisbWidth->setValue(width);
                                ui->uisbHeight->setValue(height);
                                ui->uisbDepth->setValue(depth);
                                bSize = true;
                                //QMessageBox::critical(0, "Fehler", "Okay!");
                            }
                            else
                                QMessageBox::critical(0, "Fehler", "Size-Eintrag ist fehlerhaft(size = <w> <h> <d>)");
                        }
                    }
                    if(bSize == false) QMessageBox::critical(0, "Fehler", "Size-Eintrag nicht gefunden");
                }
                else
                    QMessageBox::critical(0, "Fehler", "Kein Zeilenumbruch gefunden");
            }
            else
                QMessageBox::critical(0, "Fehler", "Datei konnte nicht gelesen werden bzw. ist leer");

            if(QFile::exists(ui->uileFile->text().left(ui->uileFile->text().length()-4) + ".vol"))
                ui->uileFileVolume->setText(ui->uileFile->text().left(ui->uileFile->text().length()-4) + ".vol");
            else if(QFile::exists(ui->uileFile->text().left(ui->uileFile->text().length()-4) + ".raw"))
                ui->uileFileVolume->setText(ui->uileFile->text().left(ui->uileFile->text().length()-4) + ".raw");
            else
                QMessageBox::critical(0, "Fehler", "Volumedatei nicht gefunden(.vol/.raw)");
        }
        else
            QMessageBox::critical(0, "Fehler", "Datei konnte nicht geöffnet werden");
    }
}
