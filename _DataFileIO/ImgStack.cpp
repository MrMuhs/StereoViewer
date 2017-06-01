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

#include "_DataFileIO/ImgStack.h"
#include <QCoreApplication>

ImgStack::ImgStack(QString sFilename, QString sFilePath, QString sFileExtension, int iIndexMin, int iIndexMax, int iIndexLength, bool bZeroFilled, int iWidth, int iHeight)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    m_sFileName = sFilename;
    m_sFilePath = sFilePath;
    m_sFileExtension = sFileExtension;
    m_iIndexMin = iIndexMin;
    m_iIndexMax = iIndexMax;
    m_iIndexLength = iIndexLength;
    m_bZeroFilled = bZeroFilled;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_bUserWantToAbort = false;

    m_iDepth = iIndexMax - iIndexMin + 1;
    m_iBytesPerTexel = 1; // Working with a Gray-Scale Texture, for first!
    //m_bReadImgStack();
}

ImgStack::~ImgStack()
{

}

bool ImgStack::m_bReadImgStack()
{
    //m_ucTexData = (unsigned char *)malloc(m_iWidth * m_iHeight * m_iDepth * m_iunsigned charsPerTexel);
    m_TexArray = new TexArray3D(m_iHeight, m_iWidth, m_iDepth, m_iBytesPerTexel);

    const char* filenumberformat;
    if(m_bZeroFilled == true)
    {
        QString str1 = "%0" + QString::number(m_iIndexLength) + "i";
        QByteArray ba = str1.toLatin1();
        filenumberformat = ba.data();
    }
    else
        filenumberformat = "%i";

    int Rot, Gruen, Blau;
    for(int ImgIndex = m_iIndexMin; ImgIndex <= m_iIndexMax; ImgIndex++)
    {
        // Check on user wants to abort reading(got for the processdialog btn)
        QCoreApplication::processEvents();
        if(m_bUserWantToAbort == true)
        {
            delete m_TexArray;
            m_TexArray = 0; // !!!
            return false;
        }

        // Put together the Filepath
        QString pfad = m_sFilePath + m_sFileName + m_sFillWithZeros(ImgIndex, m_iIndexLength) + "." + m_sFileExtension;

        double proz = 100*(ImgIndex / (double)m_iIndexMax);
        emit this->m_vDecoratorStatusChanged("Reading Imagestack", (int)proz);


        m_LogFile->m_bAppendMessage(Debug, "(ImgStack::m_bReadImgStack) Reading Img: " + pfad);

        QImage* image = new QImage();
        image->load(pfad);
        for(int s=0; s < m_iWidth; s++)
        {
            for(int t=0; t < m_iHeight; t++)
            {
                Rot = qRed(image->pixel(s,t));
                Gruen = qGreen(image->pixel(s,t));
                Blau = qBlue(image->pixel(s,t));
                //double gray = 0.299*Rot+0.587*Gruen+0.114*Blau;
                unsigned char gray = (unsigned char)((Rot+Gruen+Blau)/3.0);
                m_TexArray->m_vSetValue(t,s,ImgIndex, gray);
            }
        }
        delete image;
    }

    return true;
}

TexArray3D* ImgStack::m_GetTexArray(ProcessInfo* m_ProcessInfo)
{
    QObject::connect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
    m_bReadImgStack();
    QObject::disconnect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
    return m_TexArray;
}

void ImgStack::m_IfUserWantToAbortReadingExplizit()
{
    m_bUserWantToAbort = true;
}

QString ImgStack::m_sFillWithZeros(int value, int len)
{
    QString str1 = QString::number(value);
    while(str1.length() < len)
        str1 = "0" + str1;
    return str1;
}
