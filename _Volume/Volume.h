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

#ifndef VOLUME_H
#define VOLUME_H

#include "_Misc/LogFile.h"
#include "_DataFileIO/iDataFile.h"
#include "_OpenGL/iCamera.h"
#include "_Volume/VolumeData/VolumeData.h"
#include "_Volume/TransferFunction/iTransferFunction.h"
#include "_OpenGL/GeoCubeBase.h"

class Volume : public QObject
{
    Q_OBJECT
public:
    Volume();
    ~Volume();
    bool m_bInitializeTex();
    bool m_bInitializeTransferfunctionTex();
    bool m_bInitializeVolumeTex();
    bool m_bRenderVolume(iCamera* m_lCam);
    int m_iGetColorTableTexName();
    int m_iGetVolumeDataTexName();
    bool m_bUpdateColorTableTex();
    iTransferFunction* m_GetTransferfuction();
    bool m_bHasData();
    bool m_bSetDataFile(TexArray3D* file);
    QList<double>* m_GetHistogram();

    bool m_GlExtInitialised;
    GeoCubeBase* m_RenderCube;

signals:
    void m_vVolumeDataChanged();

private:
    LogFile* m_LogFile;
    VolumeData* m_Data;
    iTransferFunction* m_Function;
    iCamera* m_Camera;

    PFNGLMULTITEXCOORD1FARBPROC glextMultiTexCoord1fARB;
    PFNGLMULTITEXCOORD3FARBPROC glextMultiTexCoord3fARB;
    PFNGLCLIENTACTIVETEXTUREARBPROC glextActiveTextureARB;
    PFNGLACTIVETEXTUREARBPROC glextClientActiveTextureARB;
    bool m_bInitExtensions();
    double m_bGeradeSchneidetEbeneVektorForm(QVector3D ebenep1, QVector3D ebenep2, QVector3D ebenep3, QVector3D geradepunkt, QVector3D geraderichtungsvector);
    void m_vDrawDebugLines();
};

#endif // VOLUME_H
