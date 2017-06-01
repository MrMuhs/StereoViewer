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

#ifndef HISTOGRAMGENERATOR_H
#define HISTOGRAMGENERATOR_H

#include "_DataFileIO/iDataFileDeko.h"

class HistogramGenerator : public iDataFileDeko
{
public:
    HistogramGenerator(iDataFile* ToDecorated);
    virtual TexArray3D* m_GetTexArray(ProcessInfo* m_ProcessInfo);
    virtual void m_IfUserWantToAbortReadingExplizit();
    virtual void m_IfUserWantToChangeSettingsExplizit();

private:
    TexArray3D* m_TexArray;
    bool m_bUserWantToAbort;

    void m_vGenerateHistogram();
};

#endif // HISTOGRAMGENERATOR_H
