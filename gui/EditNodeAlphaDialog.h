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

#ifndef EDITNODEALPHADIALOG_H
#define EDITNODEALPHADIALOG_H

#include <QDialog>

namespace Ui {
    class EditNodeAlphaDialog;
}

class EditNodeAlphaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditNodeAlphaDialog(QWidget *parent = 0);
    ~EditNodeAlphaDialog();
    void m_vSetPreValues(unsigned char gray, unsigned char alpha);
    unsigned char m_ucGray;
    unsigned char m_ucAlpha;

public slots:
    void m_vCheckOkay();

private:
    Ui::EditNodeAlphaDialog *ui;
};

#endif // EDITNODEALPHADIALOG_H
