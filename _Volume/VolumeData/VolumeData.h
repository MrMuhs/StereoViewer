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

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <QtGui>
#include <QtOpenGL>
#include <QImage>
#include <math.h>
#include "_Misc/LogFile.h"
#include "_DataFileIO/iDataFile.h"
// compiling on windows: 3d-texture/multi-texturing stuff is defined in this
#ifdef _WIN32
    //#include <GL/glext.h>
#else
    #include <GL/glx.h> // for import function glXGetProcAddress
#endif

class VolumeData : public QObject
{
    Q_OBJECT
public:
    VolumeData();
    ~VolumeData();
    bool m_bBuildDataTexture();
    unsigned int m_uiGetTextureName();
    bool m_GlExtInitialised;
    bool m_bIsTextureInitialized();
    bool m_bHasData();
    bool m_bSetDataFile(TexArray3D* file);
    TexArray3D* m_GetDataFile();
    QList<double>* m_GetHistogram();

private:
    LogFile* m_LogFile;
    TexArray3D* m_DataFile;
    unsigned int m_uiTexName;
    bool m_bTexIsInitialized;
    bool m_bInitExtensions();

    PFNGLTEXIMAGE3DPROC glextTexImage3D;
};

#endif // RENDEROBJECT_H
