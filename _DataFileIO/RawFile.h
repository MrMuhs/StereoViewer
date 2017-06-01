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

#ifndef RAWFILE_H
#define RAWFILE_H

#include "_Misc/LogFile.h"
#include "_DataFileIO/iDataFile.h"

class RawFile : public iDataFile
{
public:
    RawFile(QString sFile, int iWidth, int iHeight, int iDepth, int iBytesPerTexel);
    ~RawFile();
    virtual TexArray3D* m_GetTexArray(ProcessInfo* m_ProcessInfo); // decorator pattern methode
    virtual void m_IfUserWantToAbortReadingExplizit();
    virtual void m_IfUserWantToChangeSettingsExplizit() {}

private:
    LogFile* m_LogFile;
    TexArray3D* m_TexArray;
    // Fields for File Infos
    QString m_sFile;
    int m_iWidth, m_iHeight, m_iDepth, m_iBytesPerTexel;
    bool m_bUserWantToAbort;

    bool m_bReadFile();
};

#endif // RAWFILE_H
