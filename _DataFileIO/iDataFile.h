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

#ifndef IDATAFILE_H
#define IDATAFILE_H

//??? #include <QtOpenGL> // required for unsigned char !?!
#include "_DataFileIO/TexArray3D.h"
#include "gui/ProcessInfo.h"

class iDataFile : public QObject
{
    Q_OBJECT
public:
    virtual ~iDataFile() {}
    virtual TexArray3D* m_GetTexArray(ProcessInfo* m_ProcessInfo) = 0;
    virtual void m_IfUserWantToAbortReadingExplizit() = 0;
    virtual void m_IfUserWantToChangeSettingsExplizit() = 0;

private slots:
    void m_vIfUserWantsToAbortReading() { m_IfUserWantToAbortReadingExplizit(); }
    void m_vIfUserWantsToChangeSettings() { m_IfUserWantToChangeSettingsExplizit(); }

signals:
    void m_vDecoratorStatusChanged(QString sMessage, int iPercent);
};

#endif // IDATAFILE_H
