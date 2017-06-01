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

#ifndef UNSHARPMASKING_H
#define UNSHARPMASKING_H

#include "_DataFileIO/iDataFileDeko.h"
#include "gui/UnsharpMaskingDialog.h"

class UnsharpMasking : public iDataFileDeko
{
public:
    UnsharpMasking(iDataFile* ToDecorated);
    virtual TexArray3D* m_GetTexArray(ProcessInfo* m_ProcessInfo);
    virtual void m_IfUserWantToAbortReadingExplizit();
    virtual void m_IfUserWantToChangeSettingsExplizit();

private:
    TexArray3D* m_TexArray;
    bool m_bUserWantToAbort;

    int m_iRadius;
    double m_dMenge;
    double m_dThreshold;
    void m_vDoUnsharpMasking();
    void m_vUnsharpImage(QImage* l_ImgIn, QImage* l_ImgOut, double l_dRadius, double l_dMenge, double l_dThreshold);
    QImage m_vBlurImage(const QImage& image, const QRect& rect, int radius, bool alphaOnly = false);
};

#endif // UNSHARPMASKING_H
