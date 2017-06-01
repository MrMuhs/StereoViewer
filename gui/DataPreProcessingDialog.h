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

#ifndef DATAPREPROCESSINGDIALOG_H
#define DATAPREPROCESSINGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "_DataFileIO/ImagePreprocessing/TabViewButtonDelegate.h"
#include "_DataFileIO/ImagePreprocessing/TabViewCheckboxDelegate.h"
#include "_DataFileIO/ImagePreprocessing/TabViewTextReadOnlyDelegate.h"
#include "_DataFileIO/ImagePreprocessing/TabViewEditButton.h"
#include "_DataFileIO/iDataFile.h"

namespace Ui {
    class DataPreProcessingDialog;
}

class DataPreProcessingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DataPreProcessingDialog(QWidget *parent = 0);
    ~DataPreProcessingDialog();
    bool m_bSetBaseFile(iDataFile* file);
    iDataFile* m_oGetResultFile();

private slots:
    void m_vButtonOkay();
    void m_vButtonCancel();
    void m_vButtonAddOperation();
    void m_vButtonRemoveOperation();
    void m_vButtonOperationMoveUp();
    void m_vButtonOperationMoveDown();

private:
    Ui::DataPreProcessingDialog *ui;
    QStandardItemModel* m_DataModel;
    iDataFile* m_BaseFile;

    bool m_bAddOperationRow(QString sFunctionName);
};

#endif // DATAPREPROCESSINGDIALOG_H
