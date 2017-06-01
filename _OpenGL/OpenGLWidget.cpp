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

#include "_OpenGL/OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::OpenGLWidget) Executing", FuncUp);

    //QGLWidget(parent, m_OpenGLWidgetGetInstanz());

    // Load Shaderressources
    {
    #ifdef SV_USE_SLICING
    QFile file(QDir::currentPath() + "/Fragment.glsl");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_sShaderGLSLCodeFrag = file.readAll();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(OpenGLWidget::OpenGLWidget) Error opening fragmentshader ressource");
    }
    file.close();
    #endif

    #ifdef SV_USE_RAYTRACING
    QFile file(QDir::currentPath() + "/FragmentRay.glsl");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_sShaderGLSLCodeFrag = file.readAll();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(OpenGLWidget::OpenGLWidget) Error opening fragmentshader(ray) ressource");
    }
    file.close();
    #endif

    file.setFileName(QDir::currentPath() + "/Vertex.glsl");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_sShaderGLSLCodeVert = file.readAll();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(OpenGLWidget::OpenGLWidget) Error opening vextexshader ressource");
    }
    file.close();
    }

    m_StereoSettings = new StereoSettings();
    m_StereoSettings->m_bStereoOn = false;
    m_StereoSettings->m_StereoMethod = Anaglyp;
    m_StereoSettings->m_dStereoBasis = 0.05;
    m_StereoSettings->m_dFernpunktweite = 15.0;

    m_vInitCameraWithDefault();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(m_setFps()));
    timer->start(500);
    m_ShaderProgramm = new QGLShaderProgram(this);
    setFocusPolicy(Qt::StrongFocus);

    m_bIAmMasterContext = false;

    GLubyte* ver = (GLubyte*)glGetString(GL_VERSION);
    QString d = (char*)ver;
    m_LogFile->m_bAppendMessage(Load, "(OpenGLWidget::OpenGLWidget) Available OpenGL Version is " + d);

//        ver = (GLuunsigned char*)glGetString(GL_EXTENSIONS);
//        d = (char*)ver;
//        m_LogFile->m_bAppendMessage(Load, "(OpenGLWidget::OpenGLWidget) Available Extensions: " + d);

    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::OpenGLWidget) OK", FuncDown);
}

OpenGLWidget::~OpenGLWidget()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::~OpenGLWidget) Executing", FuncUp);
    if(m_ShaderProgramm)
    {
        m_ShaderProgramm->removeAllShaders();
        m_ShaderProgramm->release();
        delete m_ShaderProgramm;
    }
    if(m_ShaderF) delete m_ShaderF;
    if(m_ShaderV) delete m_ShaderV;
    if(m_Camera) delete m_Camera;
    if(m_CameraStereo) delete m_CameraStereo;
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::~OpenGLWidget) OK", FuncDown);
}

void OpenGLWidget::initializeGL()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::initializeGL) Executing", FuncUp);
    #ifdef _WIN32
        HGLRC mastercontext = m_OpenGLWidgetGetInstanz();
        bool res = false;
        if(m_bIAmMasterContext == false)
        {
            res = wglShareLists((HGLRC__*)mastercontext, wglGetCurrentContext());
        }
    #else
        /*
        i tried to create one master context in the frist widged, on opening new widget the
        mastercontext is generated with a sharelist link to the static mastercontext for
        the first widget, but in this linked widget there is nothing to see, i experimented
        with this for a few weeks but without any result */
        GLXContext mastercontext = m_OpenGLWidgetGetInstanz();
        Display* dpy = glXGetCurrentDisplay();
        GLXDrawable drawable = glXGetCurrentDrawable();
        if(m_bIAmMasterContext == False)
            glXMakeCurrent(dpy, drawable, mastercontext);
    #endif

    glEnable(GL_MULTISAMPLE);

    // init shaderprogram
    m_ShaderF = new QGLShader(QGLShader::Fragment, this);
    m_ShaderF->compileSourceCode(m_sShaderGLSLCodeFrag);
    m_ShaderV = new QGLShader(QGLShader::Vertex, this);
    m_ShaderV->compileSourceCode(m_sShaderGLSLCodeVert);

    m_ShaderProgramm = new QGLShaderProgram(this);
    m_ShaderProgramm->addShader(m_ShaderF);
    m_ShaderProgramm->addShader(m_ShaderV);

    //QString QGLShaderProgram::log () const     hier könnte man noch errors lesen
	
	// @TODO: This extension/version check should be done by a extra class!
    
	// Check the Version. Needs to be >= 1.2 for glTexImage3D-function, see VolumeData::m_bInitExtensions() methode
    GLubyte* ver = (GLubyte*)glGetString(GL_VERSION);
    QString d = (char*)ver;
    if(d.left(3).toDouble() >= 1.2)
        m_LogFile->m_bAppendMessage(Load, "(OpenGLWidget::initializeGL) Available OpenGL Version is " + d);
    else
        m_LogFile->m_bAppendMessage(Error, "(OpenGLWidget::initializeGL) Available OpenGL Version is " + d + " a version >= 1.2 is required");

    // Check the extensions
    ver = (GLubyte*)glGetString(GL_EXTENSIONS);
    d = (char*)ver;
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::initializeGL) Available Extensions: " + d);

    if(d.indexOf("GL_ARB_multitexture")) // used in Volume and VolumeData, see there m_bInitExtensions() methodes
        m_LogFile->m_bAppendMessage(Load, "(OpenGLWidget::initializeGL) Found GL extension GL_ARB_multitexture");
    else
        m_LogFile->m_bAppendMessage(Error, "(OpenGLWidget::initializeGL) Not found GL extension GL_ARB_multitexture");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Set the Background-Color to black
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::initializeGL) OK", FuncDown);
}

void OpenGLWidget::paintGL()
{
    // TODO: optimize to get more frames/sec rendered
    if(m_Volume->m_bHasData() == true)
    {
        #ifndef _WIN32
            // this screews up fps, but i didnt found a way to share the textures on linux :(
            // i tried some solutions, see commented text in initializeGL()
            m_bRefreshTextures();
            //m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::paintGL) Linux sucks");
        #endif
		
		#ifdef SV_USE_RAYTRACING
        m_ShaderProgramm->release();
        // Draw some lines arround the volume texture to debugging easier...
        m_vDrawDebugLines();
		#endif

        if(m_StereoSettings->m_bStereoOn == true)
        {
            switch(m_StereoSettings->m_StereoMethod)
            {
                case Anaglyp:
                {
                    // Shading requires qt-version >= 4.6 !!! ->
                    m_ShaderProgramm->bind();
                    //QString QGLShaderProgram::log () const     hier könnte man noch errors lesen
                    m_ShaderProgramm->setUniformValue("ColorTable", 1);
                    m_ShaderProgramm->setUniformValue("VolumeTexture", 0);
                    m_ShaderProgramm->setUniformValue("PosLicht", QVector3D(0.0, 2.0, 0.0));
                    m_ShaderProgramm->setUniformValue("PosCamera", m_CameraStereo->m_GetPositionPoint());
                    m_ShaderProgramm->setUniformValue("bGrayScale", true);

                    glDrawBuffer(GL_BACK);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();

                    m_CameraStereo->m_glbSetupFrustum();
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    m_CameraStereo->m_glbSetupLookAtRightEye();

                    glPushMatrix();
                    // objekte rendern
                    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
                    m_Volume->m_bRenderVolume(m_Camera);
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glPopMatrix();
                    glFlush();



                    glDrawBuffer(GL_BACK);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();

                    m_CameraStereo->m_glbSetupFrustum();
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    m_CameraStereo->m_glbSetupLookAtLeftEye();
                    glPushMatrix();
                    // objekte rendern
                    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
                    m_Volume->m_bRenderVolume(m_Camera);
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glPopMatrix();
                    glFlush();

                    break;
                }
                case PolarisationLeft:
                {
                    // Shading requires qt-version >= 4.6 !!! ->
                    m_ShaderProgramm->bind();
                    m_ShaderProgramm->setUniformValue("ColorTable", 1);
                    m_ShaderProgramm->setUniformValue("VolumeTexture", 0);
                    m_ShaderProgramm->setUniformValue("PosLicht", QVector3D(0.0, 2.0, 0.0));
                    m_ShaderProgramm->setUniformValue("PosCamera", m_CameraStereo->m_GetPositionPoint());
                    m_ShaderProgramm->setUniformValue("bGrayScale", false);

                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glDrawBuffer(GL_BACK);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();

                    m_GlobalPolariationCamGetInstanz()->m_glbSetupFrustum();
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    m_GlobalPolariationCamGetInstanz()->m_glbSetupLookAtLeftEye();

                    glPushMatrix();
                    // objekte rendern
                    m_Volume->m_bRenderVolume(m_GlobalPolariationCamGetInstanz()->m_GetBaseCamera());
                    glPopMatrix();
                    glFlush();

                    break;
                }
                case PolarisationRight:
                {
                    // Shading requires qt-version >= 4.6 !!! ->
                    m_ShaderProgramm->bind();
                    m_ShaderProgramm->setUniformValue("ColorTable", 1);
                    m_ShaderProgramm->setUniformValue("VolumeTexture", 0);
                    m_ShaderProgramm->setUniformValue("PosLicht", QVector3D(0.0, 2.0, 0.0));
                    m_ShaderProgramm->setUniformValue("PosCamera", m_CameraStereo->m_GetPositionPoint());
                    m_ShaderProgramm->setUniformValue("bGrayScale", false);

                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    glDrawBuffer(GL_BACK);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();

                    m_GlobalPolariationCamGetInstanz()->m_glbSetupFrustum();
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    m_GlobalPolariationCamGetInstanz()->m_glbSetupLookAtRightEye();

                    glPushMatrix();
                    // objekte rendern
                    m_Volume->m_bRenderVolume(m_GlobalPolariationCamGetInstanz()->m_GetBaseCamera()); //m_Camera);
                    glPopMatrix();
                    glFlush();
                    break;
                }
            }
        }
        else
        {
            //m_ShaderProgramm->link();
            m_ShaderProgramm->bind();
            #ifdef SV_USE_RAYTRACING
			m_ShaderProgramm->setUniformValue("ColorTable", 1);
            m_ShaderProgramm->setUniformValue("VolumeTexture", 0);
            m_ShaderProgramm->setUniformValue("uv3PosLicht", QVector3D(0.0, 2.0, 0.0));
            m_ShaderProgramm->setUniformValue("ufPosCameraX", (GLfloat)m_Camera->m_GetPositionPoint().x());
            m_ShaderProgramm->setUniformValue("ufPosCameraY", (GLfloat)m_Camera->m_GetPositionPoint().y());
            m_ShaderProgramm->setUniformValue("ufPosCameraZ", (GLfloat)m_Camera->m_GetPositionPoint().z());
            m_ShaderProgramm->setUniformValue("ubGrayScale", false);
            m_ShaderProgramm->setUniformValue("ufCubeWidth", (GLfloat)m_Volume->m_RenderCube->m_dGetWidth());
            m_ShaderProgramm->setUniformValue("ufCubeHeight", (GLfloat)m_Volume->m_RenderCube->m_dGetHeight());
            m_ShaderProgramm->setUniformValue("ufCubeDepth", (GLfloat)m_Volume->m_RenderCube->m_dGetDepth());
			#elif SV_USE_SLICING
            m_ShaderProgramm->setUniformValue("ColorTable", 1);
            m_ShaderProgramm->setUniformValue("VolumeTexture", 0);
            m_ShaderProgramm->setUniformValue("PosLicht", QVector3D(0.0, 2.0, 0.0));
            m_ShaderProgramm->setUniformValue("PosCamera", m_Camera->m_GetPositionPoint());
			m_ShaderProgramm->setUniformValue("bGrayScale", false);
			#endif

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDrawBuffer(GL_BACK);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            m_Camera->m_glbSetupFrustum();

			#ifdef SV_USE_RAYTRACING
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            //glDepthFunc(GL_GREATER);
            //GL_GEQUAL
			#endif

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            m_Camera->m_glbSetupLookAt();
            glPushMatrix();
            m_Volume->m_bRenderVolume(m_Camera);
            glPopMatrix();

            glFlush();
        }
    }
}

void OpenGLWidget::resizeGL(int width, int height)
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::resizeGL) Executing", FuncUp);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_Camera->m_bCalcFrustum(width, height);
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::resizeGL) OK", FuncDown);
}

void OpenGLWidget::updateGL()
{
     update();
     m_iFramesCounter += 1;
}

bool OpenGLWidget::m_bRefreshTextures()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshTextures) Executing", FuncUp);
    m_bRefreshDataTex();
    m_bRefreshTransferfunctionTex();
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshTextures) OK", FuncDown);
    return true;
}

bool OpenGLWidget::m_bRefreshDataTex()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshDataTex) Executing", FuncUp);
    if(m_bIAmMasterContext == true || true)
    {
        m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshDataTex) (1) OK", FuncDown);
        return m_Volume->m_bInitializeVolumeTex();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshDataTex) (2) OK", FuncDown);
        return true;
    }
}

bool OpenGLWidget::m_bRefreshTransferfunctionTex()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshTransferfunctionTex) Executing", FuncUp);
    if(m_bIAmMasterContext == true || true)
    {
        m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshTransferfunctionTex) (1) OK", FuncDown);
        return m_Volume->m_bInitializeTransferfunctionTex();
    }
    else
    {
        m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_bRefreshTransferfunctionTex) (2) OK", FuncDown);
        return true;
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::mousePressEvent) Executing", FuncUp);
    if(m_StereoSettings->m_bStereoOn == true)
    {
        switch(m_StereoSettings->m_StereoMethod)
        {
            case Anaglyp: m_CameraStereo->m_bMousePressHandler(event); break;
            case PolarisationLeft: m_GlobalPolariationCamGetInstanz()->m_bMousePressHandler(event); emit m_vIChangedThePolarCam(); break;
            case PolarisationRight: m_GlobalPolariationCamGetInstanz()->m_bMousePressHandler(event); emit m_vIChangedThePolarCam(); break;
        }
    }
    else
    {
        m_Camera->m_bMousePressHandler(event);
    }
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::mousePressEvent) OK", FuncDown);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_StereoSettings->m_bStereoOn == true)
    {
        switch(m_StereoSettings->m_StereoMethod)
        {
            case Anaglyp: m_CameraStereo->m_bMouseMoveHandler(event); break;
            case PolarisationLeft: m_GlobalPolariationCamGetInstanz()->m_bMouseMoveHandler(event); emit m_vIChangedThePolarCam(); break;
            case PolarisationRight: m_GlobalPolariationCamGetInstanz()->m_bMouseMoveHandler(event); emit m_vIChangedThePolarCam(); break;
        }
    }
    else
    {
        m_Camera->m_bMouseMoveHandler(event);
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    if(m_StereoSettings->m_bStereoOn == true)
    {
        switch(m_StereoSettings->m_StereoMethod)
        {
            case Anaglyp: m_CameraStereo->m_bMouseWheelHandler(event); break;
            case PolarisationLeft: m_GlobalPolariationCamGetInstanz()->m_bMouseWheelHandler(event); emit m_vIChangedThePolarCam(); break;
            case PolarisationRight: m_GlobalPolariationCamGetInstanz()->m_bMouseWheelHandler(event); emit m_vIChangedThePolarCam(); break;
        }
    }
    else
    {
        m_Camera->m_bMouseWheelHandler(event);
    }
}
void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(m_StereoSettings->m_bStereoOn == true)
    {
        m_CameraStereo->m_bKeyPressHandler(event);
    }
    else
    {
        m_Camera->m_bKeyPressHandler(event);
    }
}

void OpenGLWidget::m_setFps()
{
    m_iFPS = m_iFramesCounter*2; // *2 beause the running timer updates 2 times a second, and we need frames per second not per 500ms!
    m_iFramesCounter = 0;
    emit m_FPSChanged();
}

void OpenGLWidget::m_vInitCameraWithDefault()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_vInitCameraWithDefault) Executing", FuncUp);
    QVector3D CamPos;     CamPos.setX(DefaultCamPosX);     CamPos.setY(DefaultCamPosY);     CamPos.setZ(DefaultCamPosZ);
    QVector3D CamTarget;  CamTarget.setX(DefaultCamTarX);  CamTarget.setY(DefaultCamTarY);  CamTarget.setZ(DefaultCamTarZ);
    QVector3D CamUp;      CamUp.setX(DefaultCamUpX);       CamUp.setY(DefaultCamUpY);       CamUp.setZ(DefaultCamUpZ);

    m_Camera = new CameraSimple(this, CamPos, CamTarget, CamUp, width(), height(), 0.1, 10.0, 45.0);
    m_CameraStereo = new CameraStereo(m_Camera, m_StereoSettings);
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_vInitCameraWithDefault) OK", FuncDown);
}

bool OpenGLWidget::m_bReCalcCamera()
{
    return m_CameraStereo->m_bCalcEyePositions();
}

CameraStereo* OpenGLWidget::m_GlobalPolariationCam = 0;

CameraStereo* OpenGLWidget::m_GlobalPolariationCamGetInstanz()
{
    if(m_GlobalPolariationCam == 0)
    {
        m_GlobalPolariationCam = new CameraStereo(m_Camera, m_StereoSettings);
    }
    return m_GlobalPolariationCam;
}

#ifdef _WIN32
HGLRC OpenGLWidget::m_StaticContext = 0;

HGLRC OpenGLWidget::m_OpenGLWidgetGetInstanz()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_OpenGLWidgetGetInstanz) Executing", FuncUp);
    if(m_StaticContext == 0)
    {
        m_StaticContext = wglGetCurrentContext();
        m_bIAmMasterContext = true;
    }
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_OpenGLWidgetGetInstanz) Executing", FuncDown);
    return m_StaticContext;
}
#else
GLXContext OpenGLWidget::m_StaticContext = 0;

GLXContext OpenGLWidget::m_OpenGLWidgetGetInstanz()
{
    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_OpenGLWidgetGetInstanz) Executing", FuncUp);
    if(m_StaticContext == 0)
    {
        m_StaticContext = glXGetCurrentContext();
        m_bIAmMasterContext = true;
    }
    /*XVisualInfo *vinf, tmp;
    long mask;
    int n;
    mask = 0;
    vinf = XGetVisualInfo(glXGetCurrentDisplay(), mask, &tmp, &n);
    glXCreateContext(glXGetCurrentDisplay(), vinf, 0, true);
    glXCreateNewContext(glXGetCurrentDisplay(),)

    m_StaticContext = glXGetCurrentContext();
    m_bIAmMasterContext = true;*/

    Display* dpy = glXGetCurrentDisplay();
    GLXContext orig_ctx = glXGetCurrentContext();

    int screen = -1;
    glXQueryContext(dpy, orig_ctx, GLX_SCREEN, &screen);
    int attribs[] = { GLX_FBCONFIG_ID, -1, None };
    int dummy;
    glXQueryContext(dpy, orig_ctx, GLX_FBCONFIG_ID, &attribs[1]);
    GLXFBConfig* fb = glXChooseFBConfig(dpy, screen, attribs, &dummy);
    XVisualInfo* vis = glXGetVisualFromFBConfig(dpy, *fb);
    //m_StaticContext = glXCreateContext(dpy, vis, orig_ctx, True);

    GLXContext bla = glXCreateContext(dpy, vis, m_StaticContext, True);

    m_LogFile->m_bAppendMessage(Debug, "(OpenGLWidget::m_OpenGLWidgetGetInstanz) OK", FuncDown);
    return bla;
}
#endif

#ifdef SV_USE_RAYTRACING
void OpenGLWidget::m_vDrawDebugLines()
{
    glColor3f(255.0, 0.0, 0.0);

    // Bottom
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackLeft().x(), m_Volume->m_RenderCube->m_GetBottomBackLeft().y(), m_Volume->m_RenderCube->m_GetBottomBackLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackRight().x(), m_Volume->m_RenderCube->m_GetBottomBackRight().y(), m_Volume->m_RenderCube->m_GetBottomBackRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontRight().x(), m_Volume->m_RenderCube->m_GetBottomFrontRight().y(), m_Volume->m_RenderCube->m_GetBottomFrontRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontLeft().x(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().y(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().z());
    glNormal3d(0.0, 1.0, 0.0);
    glEnd();

    // Top
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontLeft().x(), m_Volume->m_RenderCube->m_GetTopFrontLeft().y(), m_Volume->m_RenderCube->m_GetTopFrontLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontRight().x(), m_Volume->m_RenderCube->m_GetTopFrontRight().y(), m_Volume->m_RenderCube->m_GetTopFrontRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackRight().x(), m_Volume->m_RenderCube->m_GetTopBackRight().y(), m_Volume->m_RenderCube->m_GetTopBackRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackLeft().x(), m_Volume->m_RenderCube->m_GetTopBackLeft().y(), m_Volume->m_RenderCube->m_GetTopBackLeft().z());
    glEnd();

    // Front
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontLeft().x(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().y(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontRight().x(), m_Volume->m_RenderCube->m_GetBottomFrontRight().y(), m_Volume->m_RenderCube->m_GetBottomFrontRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontRight().x(), m_Volume->m_RenderCube->m_GetTopFrontRight().y(), m_Volume->m_RenderCube->m_GetTopFrontRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontLeft().x(), m_Volume->m_RenderCube->m_GetTopFrontLeft().y(), m_Volume->m_RenderCube->m_GetTopFrontLeft().z());
    glEnd();

    // Back
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackLeft().x(), m_Volume->m_RenderCube->m_GetBottomBackLeft().y(), m_Volume->m_RenderCube->m_GetBottomBackLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackLeft().x(), m_Volume->m_RenderCube->m_GetTopBackLeft().y(), m_Volume->m_RenderCube->m_GetTopBackLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackRight().x(), m_Volume->m_RenderCube->m_GetTopBackRight().y(), m_Volume->m_RenderCube->m_GetTopBackRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackRight().x(), m_Volume->m_RenderCube->m_GetBottomBackRight().y(), m_Volume->m_RenderCube->m_GetBottomBackRight().z());
    glNormal3d(0.0, 0.0, 1.0);
    glEnd();

    // Right
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontRight().x(), m_Volume->m_RenderCube->m_GetBottomFrontRight().y(), m_Volume->m_RenderCube->m_GetBottomFrontRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackRight().x(), m_Volume->m_RenderCube->m_GetBottomBackRight().y(), m_Volume->m_RenderCube->m_GetBottomBackRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackRight().x(), m_Volume->m_RenderCube->m_GetTopBackRight().y(), m_Volume->m_RenderCube->m_GetTopBackRight().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontRight().x(), m_Volume->m_RenderCube->m_GetTopFrontRight().y(), m_Volume->m_RenderCube->m_GetTopFrontRight().z());
    glEnd();

    // Left
    glBegin(GL_LINE_STRIP);
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomFrontLeft().x(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().y(), m_Volume->m_RenderCube->m_GetBottomFrontLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopFrontLeft().x(), m_Volume->m_RenderCube->m_GetTopFrontLeft().y(), m_Volume->m_RenderCube->m_GetTopFrontLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetTopBackLeft().x(), m_Volume->m_RenderCube->m_GetTopBackLeft().y(), m_Volume->m_RenderCube->m_GetTopBackLeft().z());
    glVertex3d(m_Volume->m_RenderCube->m_GetBottomBackLeft().x(), m_Volume->m_RenderCube->m_GetBottomBackLeft().y(), m_Volume->m_RenderCube->m_GetBottomBackLeft().z());
    glEnd();
}
#endif
