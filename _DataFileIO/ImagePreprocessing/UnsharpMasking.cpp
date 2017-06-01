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

#include "UnsharpMasking.h"

UnsharpMasking::UnsharpMasking(iDataFile* ToDecorated)
{
    this->m_DataFile = ToDecorated;
    m_bUserWantToAbort = false;
    m_iRadius = 8;
    m_dMenge = 0.10;
    m_dThreshold = 1.0;
}

TexArray3D* UnsharpMasking::m_GetTexArray(ProcessInfo* m_ProcessInfo)
{
    m_TexArray =  this->m_DataFile->m_GetTexArray(m_ProcessInfo);
    if(m_TexArray != 0)
    {
        QObject::connect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
        QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        m_vDoUnsharpMasking();
        QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserWantsToAbortReading()));
        QObject::disconnect(this, SIGNAL(m_vDecoratorStatusChanged(QString,int)), m_ProcessInfo, SLOT(m_vProcessChanged(QString,int)));
    }
    return m_TexArray;
}

void UnsharpMasking::m_IfUserWantToAbortReadingExplizit()
{
    m_bUserWantToAbort = true;
}

void UnsharpMasking::m_IfUserWantToChangeSettingsExplizit()
{
    UnsharpMaskingDialog m_Dialog;
    m_Dialog.m_bSetRadius(m_iRadius);
    m_Dialog.m_bSetMenge(m_dMenge);
    m_Dialog.m_bSetThreshold(m_dThreshold);
    if(m_Dialog.exec() == QDialog::Accepted)
    {
        m_iRadius = m_Dialog.m_iGetRadius();
        m_dMenge = m_Dialog.m_dGetMenge();
        m_dThreshold =  m_Dialog.m_dGetThreshold();
    }
}

void UnsharpMasking::m_vDoUnsharpMasking()
{
    double proz = 0;

    for(int i=0; i<m_TexArray->m_iGetDepth(); i++)
    {
        // Check on user wants to abort reading(submitted from the processdialog btn)
        if(m_bUserWantToAbort == true)
        {
            delete m_TexArray;
            m_TexArray = 0; // !!!
            return;
        }
        proz = 100*(i / (double)(m_TexArray->m_iGetDepth()));
        emit m_vDecoratorStatusChanged("Running Unscharfmaskieren", proz);

        // TODO: didnt found a typ for grayscale, in future this could be optimized in the calcuation ;)
        QImage in = QImage(m_TexArray->m_iGetWidth(), m_TexArray->m_iGetHeight(), QImage::Format_ARGB32),
               output = QImage(m_TexArray->m_iGetWidth(), m_TexArray->m_iGetHeight(), QImage::Format_ARGB32);

        // put one layer form texarry to an image
        for(int h=0; h<m_TexArray->m_iGetHeight(); h++)
        {
            for(int w=0; w<m_TexArray->m_iGetWidth(); w++)
            {
                int index = m_TexArray->m_iCalcArrayIndex(w,h,i);
                int gray = (int)m_TexArray->m_ucGetTextureData()[index];
                in.setPixel(w, h, qRgb(gray,gray,gray));
            }
        }

        // unsharp the image
        m_vUnsharpImage(&in, &output, m_iRadius, m_dMenge, m_dThreshold);

        // put it back to the texarry
        for(int h=0; h<m_TexArray->m_iGetHeight(); h++)
        {
            for(int w=0; w<m_TexArray->m_iGetWidth(); w++)
            {
                m_TexArray->m_vSetValue(h, w, i, (unsigned char)qRed(output.pixel(w, h)));
            }
        }
    }

    return;
}

void UnsharpMasking::m_vUnsharpImage(QImage* l_ImgIn, QImage* l_ImgOut, double l_dRadius, double l_dMenge, double l_dThreshold)
{
    QImage blured = m_vBlurImage(*l_ImgIn, l_ImgIn->rect(), l_dRadius);

    int Diff_R, Diff_G, Diff_B, redin, greenin, bluein, redout, greenout, blueout;
    for(int h=0; h<l_ImgIn->height(); h++)
    {
        for(int w=0; w<l_ImgIn->width(); w++)
        {
            redin = qRed(l_ImgIn->pixel(w,h));
            greenin = qGreen(l_ImgIn->pixel(w,h));
            bluein = qBlue(l_ImgIn->pixel(w,h));

            Diff_R = redin - qRed(blured.pixel(w,h));
            Diff_G = greenin - qGreen(blured.pixel(w,h));
            Diff_B = bluein - qBlue(blured.pixel(w,h));

            if(abs(2*Diff_R) < l_dThreshold) Diff_R = 0;
            if(abs(2*Diff_G) < l_dThreshold) Diff_R = 0;
            if(abs(2*Diff_B) < l_dThreshold) Diff_R = 0;

            redout = redin + l_dMenge * Diff_R;
            greenout = greenin + l_dMenge * Diff_G;
            blueout = bluein + l_dMenge * Diff_B;
            CLAMP(redout, 0, 255);
            CLAMP(greenout, 0, 255);
            CLAMP(blueout, 0, 255);

            //qDebug() << redout << " " << greenout << " " << blueout;

            l_ImgOut->setPixel(w, h, qRgb(redout, greenout, blueout));
        }
    }
}

QImage UnsharpMasking::m_vBlurImage(const QImage& image, const QRect& rect, int radius, bool alphaOnly)
{
    int tab[] = { 14, 10, 8, 6, 5, 5, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2 };
    int alpha = (radius < 1)  ? 16 : (radius > 17) ? 1 : tab[radius-1];

    QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    int r1 = rect.top();
    int r2 = rect.bottom();
    int c1 = rect.left();
    int c2 = rect.right();

    int bpl = result.bytesPerLine();
    int rgba[4];
    unsigned char* p;

    int i1 = 0;
    int i2 = 3;

    if (alphaOnly)
        i1 = i2 = (QSysInfo::ByteOrder == QSysInfo::BigEndian ? 0 : 3);

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r1) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += bpl;
        for (int j = r1; j < r2; j++, p += bpl)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c1 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p += 4;
        for (int j = c1; j < c2; j++, p += 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int col = c1; col <= c2; col++) {
        p = result.scanLine(r2) + col * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= bpl;
        for (int j = r1; j < r2; j++, p -= bpl)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    for (int row = r1; row <= r2; row++) {
        p = result.scanLine(row) + c2 * 4;
        for (int i = i1; i <= i2; i++)
            rgba[i] = p[i] << 4;

        p -= 4;
        for (int j = c1; j < c2; j++, p -= 4)
            for (int i = i1; i <= i2; i++)
                p[i] = (rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4;
    }

    return result;
}
