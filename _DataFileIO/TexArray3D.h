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

#ifndef TEXARRAY3D_H
#define TEXARRAY3D_H

#include "_Misc/LogFile.h"

class TexArray3D
{
public:
    TexArray3D(int height, int width, int depth, int bytespertexel);
    ~TexArray3D();
    void m_vSetValue(int height, int width, int depth, unsigned char value);

    int m_iGetHeight() const { return m_iHeight; }
    int m_iGetWidth() const { return m_iWidth; }
    int m_iGetDepth() const { return m_iDepth; }
    int m_iGetBytesPerTexel() const { return m_iBytesPerTexel; }
    unsigned char* m_ucGetTextureData() const { return m_aTexData; }
    QList<double>* m_GetHistogram() const;
    void m_SetHistogram(QList<double>* histogram) { m_Histogram = histogram; }
    int m_iCalcArrayIndex(int width, int height, int layer) const;

private:
    LogFile* m_LogFile;
    unsigned char* m_aTexData;
    QList<double>* m_Histogram;
    int m_iHeight;
    int m_iWidth;
    int m_iDepth;
    int m_iBytesPerTexel;
};

#endif // TEXARRAY3D_H
