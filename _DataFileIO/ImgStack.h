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

#ifndef IMGSTACK_H
#define IMGSTACK_H

#include "_Misc/LogFile.h"
#include "_DataFileIO/iDataFile.h"

class ImgStack : public iDataFile
{
public:
    ImgStack(QString sFilename, QString sFilePath, QString sFileExtension, int iIndexMin, int iIndexMax, int iIndexLength, bool bZeroFilled, int iWidth, int iHeight);
    ~ImgStack();
    virtual TexArray3D* m_GetTexArray(ProcessInfo* m_ProcessInfo); // decorator pattern methode
    virtual void m_IfUserWantToAbortReadingExplizit();
    virtual void m_IfUserWantToChangeSettingsExplizit() {}

private:
    LogFile* m_LogFile;
    TexArray3D* m_TexArray;
    // Fields for File Infos
    QString m_sFileName, m_sFilePath, m_sFileExtension;
    int m_iIndexMin, m_iIndexMax, m_iIndexLength;
    int m_iWidth, m_iHeight, m_iDepth, m_iBytesPerTexel;
    bool m_bZeroFilled;
    bool m_bUserWantToAbort;

    bool m_bReadImgStack();
    QString m_sFillWithZeros(int value, int len);
};

#endif // IMGSTACK_H
