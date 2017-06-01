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

#include "DataPreProcessingDialog.h"
#include "ui_DataPreProcessingDialog.h"

DataPreProcessingDialog::DataPreProcessingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataPreProcessingDialog)
{
    ui->setupUi(this);
    m_BaseFile = 0;

    ui->uicboNewOperation->addItem("Medianfilter");
    ui->uicboNewOperation->addItem("Histogramm");
    ui->uicboNewOperation->addItem("Unscharfmaskieren");

    m_DataModel = new QStandardItemModel(0, 3);
    ui->tableView->setModel(m_DataModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // qt4.8 setResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    TabViewTextReadOnlyDelegate* delegate0 = new TabViewTextReadOnlyDelegate();
    ui->tableView->setItemDelegateForColumn(0, delegate0);

    TabViewCheckboxDelegate* delegate1 = new TabViewCheckboxDelegate();
    ui->tableView->setItemDelegateForColumn(1, delegate1);
    ui->tableView->setColumnWidth(1, 30);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed); // qt4.8 setResizeMode(1, QHeaderView::Fixed);

    TabViewButtonDelegate* delegate2 = new TabViewButtonDelegate();
    ui->tableView->setItemDelegateForColumn(2, delegate2);
    ui->tableView->setColumnWidth(2, 80);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed); // qt4.8 setResizeMode(1, QHeaderView::Fixed);

    // this overrides the resizemodes !? so dont use it ;)
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);

    m_bAddOperationRow("Histogramm");

    ui->tableView->show();

    QObject::connect(ui->uibtnOkay, SIGNAL(clicked()), this, SLOT(m_vButtonOkay()));
    QObject::connect(ui->uibtnCancel, SIGNAL(clicked()), this, SLOT(m_vButtonCancel()));
    QObject::connect(ui->uibtnAdd, SIGNAL(clicked()), this, SLOT(m_vButtonAddOperation()));
    QObject::connect(ui->uibtnRemove, SIGNAL(clicked()), this, SLOT(m_vButtonRemoveOperation()));
    QObject::connect(ui->uibtnMoveUp, SIGNAL(clicked()), this, SLOT(m_vButtonOperationMoveUp()));
    QObject::connect(ui->uibtnMoveDown, SIGNAL(clicked()), this, SLOT(m_vButtonOperationMoveDown()));
}

DataPreProcessingDialog::~DataPreProcessingDialog()
{
    delete ui;
}

void DataPreProcessingDialog::m_vButtonOkay()
{
    // built the decorated objekt here !?
    if(m_BaseFile != 0)
    {
        TabViewEditButton* wtf;
        QModelIndex index;
        for(int i=0; i < m_DataModel->rowCount(); i++)
        {
            index = m_DataModel->index(i, 1, QModelIndex());
            if(index.data(Qt::BackgroundRole).toBool() == true)
            {
                index = m_DataModel->index(i, 2, QModelIndex());
                wtf = (TabViewEditButton*)ui->tableView->indexWidget(index);
                wtf->m_Operation->m_SetObjektToDekorateLaterBind(m_BaseFile);
                m_BaseFile = wtf->m_Operation;
            }
        }
        accept();
    }
    else
    {
        reject();
    }
}

void DataPreProcessingDialog::m_vButtonCancel()
{
    reject();
}

void DataPreProcessingDialog::m_vButtonAddOperation()
{
    m_bAddOperationRow(ui->uicboNewOperation->currentText());
}

void DataPreProcessingDialog::m_vButtonRemoveOperation()
{
    if(m_DataModel->rowCount() > 0 && ui->tableView->currentIndex().row() >= 0)
    {
        m_DataModel->removeRow(ui->tableView->currentIndex().row());
    }
}

void DataPreProcessingDialog::m_vButtonOperationMoveUp()
{
    int row = ui->tableView->currentIndex().row();
    if(row > 0)
    {
        m_DataModel->insertRow(row-1, m_DataModel->takeRow(row));
        QModelIndex index = m_DataModel->index(row-1, 1, QModelIndex());
        ui->tableView->openPersistentEditor(index);
        index = m_DataModel->index(row-1, 2, QModelIndex());
        ui->tableView->openPersistentEditor(index);
        ui->tableView->setCurrentIndex(index);
    }
}

void DataPreProcessingDialog::m_vButtonOperationMoveDown()
{
    int row = ui->tableView->currentIndex().row();
    if(row < m_DataModel->rowCount()-1)
    {
        m_DataModel->insertRow(row+1, m_DataModel->takeRow(row));
        QModelIndex index = m_DataModel->index(row+1, 1, QModelIndex());
        ui->tableView->openPersistentEditor(index);
        index = m_DataModel->index(row+1, 2, QModelIndex());
        ui->tableView->openPersistentEditor(index);
        ui->tableView->setCurrentIndex(index);
    }
}

bool DataPreProcessingDialog::m_bSetBaseFile(iDataFile* file)
{
    m_BaseFile = file;
    return true;
}

iDataFile* DataPreProcessingDialog::m_oGetResultFile()
{
    return m_BaseFile;
}

bool DataPreProcessingDialog::m_bAddOperationRow(QString sFunctionName)
{
    int row = ui->tableView->currentIndex().row();
    if(row < 0)
    {
        m_DataModel->appendRow(new QStandardItem(1,3));
        row = m_DataModel->rowCount() - 1;
    }
    else
        m_DataModel->insertRow(row, new QStandardItem(1,3));

    for (int column = 0; column < 3; column++)
    {
        // when modifing to a dll featured methode, have a look at the
        // m_vButtonOkay() function... so u can get the m_Operation object!
        // thats maybe the better way to set it, but for first this is a
        // working solution, so the operation will be set by the delegate.

        QModelIndex index = m_DataModel->index(row, column, QModelIndex());
        if(column == 0) m_DataModel->setData(index, QVariant(sFunctionName));
        if(column == 1) m_DataModel->setData(index, true, Qt::BackgroundRole);
        if(column == 2) m_DataModel->setData(index, QVariant(sFunctionName));
        if(column == 1 || column == 2)
        {
            // show the editor permanent
            // http://www.qtforum.org/article/24195/qpushbutton-as-delegate-in-a-qtableview.html
            ui->tableView->openPersistentEditor(index);
        }
    }
    return true;
}
