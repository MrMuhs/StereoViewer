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

#ifndef DATALOADER_H
#define DATALOADER_H

#include <QThread>
#include "_DataFileIO/iDataFile.h"
#include "gui/ProcessInfo.h"

//! DataLoader
/*!
  This class is used to load a decorator structur(for example ImgStack+MedianFilter) into a TexArray3D object. The class inherits from QThread, the loading progress is running in a background thread.
*/
class DataLoader : public QThread
{
    Q_OBJECT
public:
    /*!
      Creates a new instance from the process dialog to show the progress of work.
      \par ToLoad A pointer to the first element of the decorator, joint together form the loading dialogs classes.
    */
    DataLoader(iDataFile* ToLoad);

    /*!
      Deletes the initial given decorator structur and the instance of the processinfo dialog.
    */
    ~DataLoader();

    /*!
      Starts the thread methode run() form an external object.
      \sa run()
    */
    void m_vStartLoading();

    /*!
        \return Pointer to the loading result.
    */
    TexArray3D* m_GetLoaderResult() { return m_Result; }

public slots:
    /*!
        This slot is finally triggered form the abort signal of the processinfo dialog.
    */
    void m_vIfUserClickedAbort();

private:
    /*!
       The field keeps a pointer to the instance of the processinfo dialog object.
    */
    ProcessInfo* m_ProcessInfo;

    /*!
       The field keeps a pointer to the instance of the decorator structur object.
    */
    iDataFile* m_TargetToLoad;

    /*!
       The field keeps a pointer to the instance of the loading result object.
    */
    TexArray3D* m_Result;

    /*!
       This methode is the executed function of the background thread. It shows the processinfo dialog and calls the first object in the decorator structur to start working.
    */
    void run();
};

#endif // DATALOADER_H
