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

#include "TexArray3D.h"

TexArray3D::TexArray3D(int height, int width, int depth, int bytespertexel)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::TexArray3D) Executing");
    m_iHeight = height;
    m_iWidth = width;
    m_iDepth = depth;
    m_iBytesPerTexel = bytespertexel;

    m_Histogram = new QList<double>();
    m_aTexData = (unsigned char*)malloc(m_iHeight * m_iWidth * m_iDepth * m_iBytesPerTexel);
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::TexArray3D) OK");
}

TexArray3D::~TexArray3D()
{
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::~TexArray3D) Executing");
    if(m_Histogram) delete m_Histogram;
    if(m_aTexData) delete m_aTexData;
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::~TexArray3D) OK");
}

void TexArray3D::m_vSetValue(int height, int width, int depth, unsigned char value)
{
    m_aTexData[m_iCalcArrayIndex(width, height, depth)] = value;
}

QList<double>* TexArray3D::m_GetHistogram() const
{
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::m_GetHistogram) Executing");
    m_LogFile->m_bAppendMessage(Debug, "(TexArray3D::m_GetHistogram) OK");
    return m_Histogram;
}

int TexArray3D::m_iCalcArrayIndex(int width, int height, int layer) const
{
    return ((m_iBytesPerTexel*(width * m_iWidth + height))+(m_iWidth*m_iHeight*layer*m_iBytesPerTexel));
}

