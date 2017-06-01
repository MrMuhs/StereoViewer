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

#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QWidget>
#include <QTimer>

namespace Ui {
    class ProcessInfo;
}

//! ProcessInfo
/*!
  This class is used to show a load/process/... progress. It is expitzit used by the image preprocessing and the loading.
*/
class ProcessInfo : public QWidget
{
    Q_OBJECT

public:
    /*!
        \param parent A pointer to the parent widget.
        \param stitle A constant text shown in the title of the processbar.
        \return instanz
        \sa ~ProcessInfo()
    */
    explicit ProcessInfo(QWidget *parent = 0, QString stitle = "Prozessinfo");

    /*!
        Deletes the fields ui and m_SecondsCountUpTimer.
        \sa ProcessInfo()
    */
    ~ProcessInfo();

    /*!
        Is a inherited methode from QWidget, will be triggered by closing the widget form window system. Stops the Timer(m_SecondsCountUpTimer) and fires the signal m_vUserClickAbort() for the working classes(like ImgStack).
    */
    void closeEvent(QCloseEvent *event);

signals:
    /*!
        This signal triggered by the slot m_vUserClickAbortIntern(). It has to be used by the working classes(like ImgStack) to stop there processing.
    */
    void m_vUserClickAbort();

public slots:
    /*!
        This slot is triggered form the working classes. I case of the image preprocessing the methode iDataFile::m_vDecoratorStatusChanged() will trigger this slot, submitted down in the structur for a child class(like ImgStack).!
        \param msg A constant text shown in on the UI of the dialog for user inforamtion whats going on.
        \param percentage A constant text shown in the title of the processbar.
    */
    void m_vProcessChanged(QString msg, int percentage);

private slots:
    /*!
       This slot is triggered form the abort-button on the UI of the dialog.
    */
    void m_vUserClickAbortIntern();

    /*!
       This slot is triggered form the timer(m_SecondsCountUpTimer).
    */
    void m_vTimerUpdate();

private:
    /*!
       The field keeps a pointer to the instance of the UI object.
    */
    Ui::ProcessInfo *ui;

    /*!
       The field keeps a pointer to the instance of a QTimer object. It is finally used to approximate the left time to finish the progress.
    */
    QTimer* m_SecondsCountUpTimer;

    /*!
       The field keeps the value in seconds till the percentage count of progress was 0.
    */
    long m_iSecondsSinceZeroPercent;

    /*!
       The messagetext shown in the label field of the form
    */
    QString m_sMessage;
};

#endif // PROCESSINFO_H
