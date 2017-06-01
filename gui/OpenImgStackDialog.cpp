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

#include "gui/OpenImgStackDialog.h"
#include "ui_OpenImgStackDialog.h"

iDataFile* OpenImgStackDialog::m_ImgStack;

OpenImgStackDialog::OpenImgStackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenImgStackDialog)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    ui->setupUi(this);
    m_ImgStack = 0;
    QObject::connect(ui->uiSelectFile, SIGNAL(clicked()), this, SLOT(m_vSelectFile()));
    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vButtonOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
}

OpenImgStackDialog::~OpenImgStackDialog()
{
    delete ui;
}

void OpenImgStackDialog::changeEvent(QEvent *e)
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

void OpenImgStackDialog::m_vSelectFile()
{
    QString sFile = QFileDialog::getOpenFileName(this, "Datei öffnen", QDir::currentPath(), "Dokumente (*.tif)");
    if (!sFile.isEmpty())
    {
        QString sFileReverse = strreverse(sFile);

        bool bZeros;
        QString sFilename, sFilePath, sFileExt;
        int iIndexMin, iIndexMax, iIndexLength, iWidth, iHeight;

        QString sFilenameConstPart, stmp;

        sFileExt = strreverse(sFileReverse.left(sFileReverse.indexOf(".")));
        sFilename = strreverse(sFileReverse.left(sFileReverse.indexOf("/")).remove(0, sFileReverse.left(sFileReverse.indexOf("/")).indexOf(".")+1));
        sFilePath = strreverse(sFileReverse.remove(0, sFileReverse.indexOf("/")));

        // get const part of filename
        stmp = strreverse(sFilename);
        iIndexLength = 0;
        while(stmp.left(1) == "0" || stmp.left(1) == "1" ||
              stmp.left(1) == "2" || stmp.left(1) == "3" ||
              stmp.left(1) == "4" || stmp.left(1) == "5" ||
              stmp.left(1) == "6" || stmp.left(1) == "7" ||
              stmp.left(1) == "8" || stmp.left(1) == "9")
        {
            // maybe the first file is selected so check the first char
            if(iIndexLength == 0)
            {
                if(stmp.left(0) == 0) iIndexMin = 0; else iIndexMin = 1;
            }
            // check the filename for filled zeros
            if(iIndexLength > 0)
                if(stmp.left(0) == 0) bZeros = true;
            stmp.remove(0, 1);
            iIndexLength++;
        }

        sFilenameConstPart = strreverse(stmp);

        // get index max value by counting the files at the given path
        QDir thedir = QDir(sFilePath);
        thedir.setFilter(QDir::Files);
        iIndexMax = thedir.count() - 1; // Anzahl der dateien im path

        // get image height and width
        QImage theimg = QImage(sFile);
        iWidth = theimg.width();
        iHeight = theimg.height();

        // fill the interface with the values
        ui->uicbZeroFill->setChecked(bZeros);
        ui->uileExtension->setText(sFileExt);
        ui->uileFileName->setText(sFilenameConstPart);
        ui->uileFilePath->setText(sFilePath);
        ui->uisbHeight->setValue(iHeight);
        ui->uisbIndexLength->setValue(iIndexLength);
        ui->uisbIndexMax->setValue(iIndexMax);
        ui->uisbIndexMin->setValue(iIndexMin);
        ui->uisbWidth->setValue(iWidth);
    }
}

void OpenImgStackDialog::m_vButtonOkay()
{
    bool bZeros;
    QString sFilename, sFilePath, sFileExt;
    int iIndexMin, iIndexMax, iIndexLength, iWidth, iHeight;

    bZeros = ui->uicbZeroFill->isChecked();
    sFilename = ui->uileFileName->text();
    sFilePath = ui->uileFilePath->text();
    sFileExt = ui->uileExtension->text();
    iIndexMin = ui->uisbIndexMin->value();
    iIndexMax = ui->uisbIndexMax->value();
    iIndexLength = ui->uisbIndexLength->value();
    iWidth = ui->uisbWidth->value();
    iHeight = ui->uisbHeight->value();

    //TODO: check inputs ;)

    m_ImgStack = new ImgStack(sFilename, sFilePath, sFileExt,
                            iIndexMin, iIndexMax, iIndexLength, bZeros,
                            iWidth, iHeight);

    DataPreProcessingDialog m_Dialog;
    m_Dialog.m_bSetBaseFile(m_ImgStack);
    if(m_Dialog.exec() == QDialog::Accepted)
    {
        // get decorated object back!
        m_ImgStack = m_Dialog.m_oGetResultFile();
    }

    accept();
}

void OpenImgStackDialog::m_vButtonCancel()
{
    this->m_ImgStack = 0;
    this->reject();
}
