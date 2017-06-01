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

#include "HistogramGenerator.h"
#include <QCoreApplication>
#include <QMessageBox>

HistogramGenerator::HistogramGenerator(iDataFile* ToDecorated)
{
    this->m_DataFile = ToDecorated;
    m_bUserWantToAbort = false;
}

TexArray3D* HistogramGenerator::m_GetTexArray(ProcessInfo* m_ProcessInfo)
{
    m_TexArray =  this->m_DataFile->m_GetTexArray(m_ProcessInfo);
    if(m_TexArray != 0)
    {
        QObject::connect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
        QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        m_vGenerateHistogram();
        QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        QObject::disconnect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    }
    return m_TexArray;
}

void HistogramGenerator::m_IfUserWantToAbortReadingExplizit()
{
    m_bUserWantToAbort = true;
}

void HistogramGenerator::m_IfUserWantToChangeSettingsExplizit()
{
    QMessageBox::information(0, "Keine Einstellungen", "Histogramm hat keine Einstellungen");
}

void HistogramGenerator::m_vGenerateHistogram()
{
    double tmpHisto[256];
    for(int i = 0; i<256; i++) tmpHisto[i] = 0;

    int maxDep = m_TexArray->m_iGetDepth();
    int maxWid = m_TexArray->m_iGetWidth();
    int maxHei = m_TexArray->m_iGetHeight();

    unsigned char* TexData = m_TexArray->m_ucGetTextureData();
    for(int d=0; d<maxDep; d++)
    {
        // Check on user wants to abort reading(got for the processdialog btn)
        QCoreApplication::processEvents();
        if(m_bUserWantToAbort == true)
        {
            delete m_TexArray;
            m_TexArray = 0; // !!!
            return;
        }
        double proz = 100*(d / (double)maxDep);
        emit m_vDecoratorStatusChanged("Running HistogramGenerator", proz);

        for(int w=0; w<maxWid; w++)
        {
            for(int h=0; h<maxHei; h++)
            {
                int index =  m_TexArray->m_iCalcArrayIndex(w, h, d);
                tmpHisto[(int)TexData[index]]++;
            }
        }
    }

    QList<double>* m_Histogram = new QList<double>();
    for(int i = 0; i<256; i++) m_Histogram->append(tmpHisto[i]);
    m_TexArray->m_SetHistogram(m_Histogram);

    return;
}
