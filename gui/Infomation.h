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

#ifndef INFOMATION_H
#define INFOMATION_H

#include <QDialog>

namespace Ui {
    class Infomation;
}

class Infomation : public QDialog {
    Q_OBJECT
public:
    Infomation(QWidget *parent = 0);
    ~Infomation();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Infomation *ui;
};

#endif // INFOMATION_H
