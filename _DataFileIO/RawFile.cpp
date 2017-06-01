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

#include "RawFile.h"
#include <QCoreApplication>

RawFile::RawFile(QString sFile, int iWidth, int iHeight, int iDepth, int iBytesPerTexel)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_sFile = sFile;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iDepth = iDepth;
    m_iBytesPerTexel = iBytesPerTexel;

    m_bUserWantToAbort = false;
}

RawFile::~RawFile()
{

}

TexArray3D* RawFile::m_GetTexArray(ProcessInfo* m_ProcessInfo)
{
    QObject::connect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
    m_bReadFile();
    QObject::disconnect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
    return m_TexArray;
}

void RawFile::m_IfUserWantToAbortReadingExplizit()
{
    m_bUserWantToAbort = true;
}

bool RawFile::m_bReadFile()
{
    QFile data(m_sFile);
    if (data.open(QFile::ReadOnly))
    {
        QDataStream stream(&data);
        m_TexArray = new TexArray3D(m_iHeight, m_iWidth, m_iDepth, m_iBytesPerTexel);
        //int bytesread;
        char *buffer = new char[2];
        unsigned int x1=0;
        unsigned int x2=0;
        unsigned char val = 0;
        for(int d=0; d<m_iDepth; d++)
        {
            // Check on user wants to abort reading(got for the processdialog btn)
            QCoreApplication::processEvents();
            if(m_bUserWantToAbort == true)
            {
                delete m_TexArray;
                m_TexArray = 0; // !!!
                return false;
            }

            double proz = 100*(d / (double)m_iDepth);
            emit this->m_vDecoratorStatusChanged("Reading Rawfile", (int)proz);

            for(int h=0; h<m_iHeight; h++)
            {
                for(int w=0; w<m_iWidth; w++)
                {
                    if(!stream.atEnd())
                    {
                        buffer[0] = 0;
                        buffer[1] = 0;
                        x1=0;
                        x2=0;
                        val=0;
                        //bytesread = stream.readRawData(buffer, 2);
                        stream.readRawData(buffer, 2);

                        x1 = abs(buffer[0]);
                        x2 = abs(buffer[1]);
                        if(x1 < 0) x1 = x1 + 128;
                        if(x2 < 0) x2 = x2 + 128;
                        val = (unsigned char)255*((x2*512+x1)/65536.0);

                        m_TexArray->m_vSetValue(h, w, d, val);
                    }
                }
            }
        }
        delete [] buffer;
        return true;
    }
    return false;
}
