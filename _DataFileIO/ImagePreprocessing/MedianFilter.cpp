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

#include "MedianFilter.h"

MedianFilter::MedianFilter(iDataFile* ToDecorated)
{
    this->m_DataFile = ToDecorated;
    m_bUserWantToAbort = false;
    m_iCubeSize = 3;
}

TexArray3D* MedianFilter::m_GetTexArray(ProcessInfo* m_ProcessInfo)
{
    m_TexArray =  this->m_DataFile->m_GetTexArray(m_ProcessInfo);
    if(m_TexArray != 0)
    {
        QObject::connect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
        QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        m_vCalcMedianFilter();
        QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        QObject::disconnect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    }
    return m_TexArray;
}

void MedianFilter::m_IfUserWantToAbortReadingExplizit()
{
    m_bUserWantToAbort = true;
}

void MedianFilter::m_IfUserWantToChangeSettingsExplizit()
{
    MedianFilterDialog m_Dialog;
    m_Dialog.m_bSetBlockSize(m_iCubeSize);
    if(m_Dialog.exec() == QDialog::Accepted)
    {
        m_iCubeSize = m_Dialog.m_iGetBlockSize();
    }
}

void MedianFilter::m_vCalcMedianFilter()
{
    int maxDep = m_TexArray->m_iGetDepth();
    int maxWid = m_TexArray->m_iGetWidth();
    int maxHei = m_TexArray->m_iGetHeight();
    unsigned char* TexData = m_TexArray->m_ucGetTextureData();

    TexArray3D* TexDataOut = new TexArray3D(maxHei, maxWid, maxDep, m_TexArray->m_iGetBytesPerTexel());
    double proz = 0;
    int index;
    int cubesize = m_iCubeSize;
    int cubesizehalf = (int) cubesize/2;
    //unsigned char arr[cubesize*cubesize*cubesize]; // 27 values
    unsigned char* arr = new unsigned char[cubesize*cubesize*cubesize];
    for(int d=cubesizehalf; d<maxDep-cubesizehalf; d++)
    {
        // Check on user wants to abort reading(submitted from the processdialog btn)
        if(m_bUserWantToAbort == true)
        {
            delete m_TexArray;
            m_TexArray = 0; // !!!
            delete[] arr;
            return;
        }
        proz = 100*(d / (double)(maxDep-cubesizehalf));
        emit m_vDecoratorStatusChanged("Running Medianfilter", proz);

        for(int w=cubesizehalf; w<maxWid-cubesizehalf; w++)
        {
            for(int h=cubesizehalf; h<maxHei-cubesizehalf; h++)
            {
                // build the cubic median here
                index = 0;
                for(int di=d-cubesizehalf; di<=(d+cubesizehalf); di++)
                {
                    for(int wi=w-cubesizehalf; wi<=(w+cubesizehalf); wi++)
                    {
                        for(int hi=h-cubesizehalf; hi<=(h+cubesizehalf); hi++)
                        {
                            arr[index] = TexData[m_TexArray->m_iCalcArrayIndex(wi, hi, di)];
                            index++;
                        }
                    }
                }

/*//                arr[0] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h+1, d-1)];
//                arr[1] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h+1, d-1)];
//                arr[2] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h+1, d-1)];
//                arr[3] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h, d-1)];
//                arr[4] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h, d-1)];
//                arr[5] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h, d-1)];
//                arr[6] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h-1, d-1)];
//                arr[7] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h-1, d-1)];
//                arr[8] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h-1, d-1)];

//                arr[9] =  TexData[m_TexArray->m_iCalcArrayIndex(w-1, h+1, d)];
//                arr[10] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h+1, d)];
//                arr[11] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h+1, d)];
//                arr[12] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h, d)];
//                arr[13] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h, d)];
//                arr[14] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h, d)];
//                arr[15] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h-1, d)];
//                arr[16] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h-1, d)];
//                arr[17] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h-1, d)];

//                arr[18] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h+1, d+1)];
//                arr[19] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h+1, d+1)];
//                arr[20] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h+1, d+1)];
//                arr[21] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h, d+1)];
//                arr[22] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h, d+1)];
//                arr[23] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h, d+1)];
//                arr[24] = TexData[m_TexArray->m_iCalcArrayIndex(w-1, h-1, d+1)];
//                arr[25] = TexData[m_TexArray->m_iCalcArrayIndex(w,   h-1, d+1)];
//                arr[26] = TexData[m_TexArray->m_iCalcArrayIndex(w+1, h-1, d+1)];*/

                //m_vBubbleSort(arr, 3*3*3);
                m_vHeapSort(arr, cubesize*cubesize*cubesize);

                TexDataOut->m_vSetValue(h, w, d, arr[((int)((cubesize*cubesize*cubesize)/2))+1]);
            }
        }
    }
    delete[] arr;

    // Copy borders
    for(int borWid=0; borWid<cubesizehalf; borWid++)
    {
        for(int i=0; i<maxWid; i++)
        {
            for(int j=0; j<maxHei; j++)
            {
                TexDataOut->m_vSetValue(j, i, 0+borWid, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(i, j, 1+borWid)]);
                TexDataOut->m_vSetValue(j, i, maxDep-1-borWid, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(i, j, maxDep-2-borWid)]);
            }
        }
    }
    for(int borWid=0; borWid<cubesizehalf; borWid++)
    {
        for(int i=0; i<maxDep; i++)
            {
            for(int j=0; j<maxHei; j++)
            {
                TexDataOut->m_vSetValue(j, 0+borWid, i, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(1+borWid, j, i)]);
                TexDataOut->m_vSetValue(j, maxWid-1-borWid, i, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(maxWid-2-borWid, j, i)]);
            }
        }
    }
    for(int borWid=0; borWid<cubesizehalf; borWid++)
    {
        for(int i=0; i<maxDep; i++)
        {
            for(int j=0; j<maxWid; j++)
            {
                TexDataOut->m_vSetValue(0+borWid, j, i, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(j+borWid, 1, i)]);
                TexDataOut->m_vSetValue(maxHei-1-borWid, j, i, TexDataOut->m_ucGetTextureData()[TexDataOut->m_iCalcArrayIndex(j, maxHei-2-borWid, i)]);
            }
        }
    }

    delete m_TexArray;
    m_TexArray = TexDataOut;
}

void MedianFilter::m_vBubbleSort(unsigned char* arr, int size)
{
    // Bubble-Sorting the List
    int i, j;
    unsigned char temp;
    for (i = (size - 1); i > 0; i--)
    {
        for (j = 1; j <= i; j++)
        {
            if (arr[j-1] > arr[j])
            {
                temp = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void MedianFilter::m_vHeapSort(unsigned char* arr, int size)
{
    // Heapsort, see Numerical-Recipies
    unsigned long i,ir,j,l;
    float rra;
    if (size < 2) return;
    l=(size >> 1)+1;
    ir=size;

    for (;;)
    {
        if (l > 1)
        {
            rra=arr[--l];
        }
        else
        {
            rra=arr[ir];
            arr[ir]=arr[1];
            if (--ir == 1)
            {
                arr[1]=rra;
                break;
            }
        }
        i=l;
        j=l+l;
        while (j <= ir)
        {
            if (j < ir && arr[j] < arr[j+1]) j++;
            if (rra < arr[j])
            {
                arr[i]=arr[j];
                i=j;
                j <<= 1;
            }
            else
                break;
        }
        arr[i]=rra;
    }
}
