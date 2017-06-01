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

#ifndef STEREOTESTWID_H
#define STEREOTESTWID_H

#include <math.h>
#include <QtGui>
#include <QtOpenGL>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QDebug>
#include "_Misc/LogFile.h"
#include "_Volume/Volume.h"
#include "_OpenGL/iCamera.h"
#include "_OpenGL/CameraSimple.h"
#include "_OpenGL/CameraStereo.h"
#ifndef _WIN32
    #include <GL/glx.h> // glX... functions for rendercontext management on X server
#endif

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
    bool m_bReCalcCamera();
    bool m_bRefreshTextures();
    bool m_bRefreshDataTex();
    bool m_bRefreshTransferfunctionTex();
    void initializeGL();
    void updateGL();
    void paintGL();

    Volume* m_Volume;
    iCamera* m_Camera;
    CameraStereo* m_CameraStereo;
    StereoSettings* m_StereoSettings;
    QGLShaderProgram* m_ShaderProgramm;
    QGLShader* m_ShaderF;
    QGLShader* m_ShaderV;
    QString m_sShaderGLSLCodeFrag;
    QString m_sShaderGLSLCodeFragRay;
    QString m_sShaderGLSLCodeVert;
    int m_iFPS;

signals:
    void m_FPSChanged();
    void m_vIChangedThePolarCam();

protected:
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void m_setFps();

private:
    LogFile* m_LogFile;

    int m_iFramesCounter;
    bool m_bIAmMasterContext;
    void m_vInitCameraWithDefault();
	#ifdef SV_USE_RAYTRACING
    void m_vDrawDebugLines();
	#endif
	
    CameraStereo* m_GlobalPolariationCamGetInstanz();
    static CameraStereo* m_GlobalPolariationCam;

    #ifdef _WIN32
    HGLRC m_OpenGLWidgetGetInstanz();
    static HGLRC m_StaticContext;
    #else
    GLXContext m_OpenGLWidgetGetInstanz();
    static GLXContext m_StaticContext;
    #endif
};

#endif // STEREOTESTWID_H
