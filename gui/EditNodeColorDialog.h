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

#ifndef EDITNODECOLORDIALOG_H
#define EDITNODECOLORDIALOG_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
    class EditNodeColorDialog;
}

class EditNodeColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNodeColorDialog(QWidget *parent = 0);
    ~EditNodeColorDialog();
    void m_vSetPreValues(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue);
    unsigned char m_ucGray;
    unsigned char m_ucRed;
    unsigned char m_ucGreen;
    unsigned char m_ucBlue;

public slots:
    void m_vCheckOkay();
    void m_vSelectColor();
    void m_vColorRedChanged();
    void m_vColorGreenChanged();
    void m_vColorBlueChanged();

private:
    Ui::EditNodeColorDialog *ui;

    void m_vSetResColor(unsigned char red, unsigned char green, unsigned char blue);
};

#endif // EDITNODECOLORDIALOG_H
