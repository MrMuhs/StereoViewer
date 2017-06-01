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

#include "_Volume/Volume.h"
#include "_Volume/TransferFunction/TransferFunctionRGBA.h"

Volume::Volume()
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(Volume::Volume) Executing", FuncUp);
    m_Data = new VolumeData;
    m_Function = new TransferFunctionRGBA;
    m_RenderCube = 0;
    m_GlExtInitialised = false;

    // Für Tetraeder
//    m_Function->m_bColorNodeAdd(0, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(142, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(143, 255, 0, 0);
//    m_Function->m_bColorNodeAdd(147, 255, 0, 0);
//    m_Function->m_bColorNodeAdd(148, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(255, 0, 0, 0);
//
//    m_Function->m_bAlphaNodeAdd(0, 2);
//    m_Function->m_bAlphaNodeAdd(142, 2);
//    m_Function->m_bAlphaNodeAdd(143, 255);
//    m_Function->m_bAlphaNodeAdd(147, 255);
//    m_Function->m_bAlphaNodeAdd(148, 2);
//    m_Function->m_bAlphaNodeAdd(255, 2);

    // Für Biene
//    m_Function->m_bColorNodeAdd(0, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(150, 255, 0, 100);
//    m_Function->m_bColorNodeAdd(166, 255, 0, 255);
//    m_Function->m_bColorNodeAdd(173, 255, 0, 100);
//    m_Function->m_bColorNodeAdd(255, 0, 0, 0);
//
//    m_Function->m_bAlphaNodeAdd(0, 0);
//    m_Function->m_bAlphaNodeAdd(149, 0);
//    m_Function->m_bAlphaNodeAdd(150, 10);
//    m_Function->m_bAlphaNodeAdd(166, 30);
//    m_Function->m_bAlphaNodeAdd(173, 30);
//    m_Function->m_bAlphaNodeAdd(174, 0);
//    m_Function->m_bAlphaNodeAdd(255, 0);

    // Für zweifarbige Tetraeader
    m_Function->m_bColorNodeAdd(0, 0, 0, 0);
    m_Function->m_bColorNodeAdd(67, 0, 0, 0);
    m_Function->m_bColorNodeAdd(68, 200, 0, 200);
    m_Function->m_bColorNodeAdd(93, 255, 0, 255);
    m_Function->m_bColorNodeAdd(130, 200, 0, 200);
    m_Function->m_bColorNodeAdd(131, 0, 0, 0);
    m_Function->m_bColorNodeAdd(132, 0, 0, 0);
    m_Function->m_bColorNodeAdd(133, 100, 100, 0);
    m_Function->m_bColorNodeAdd(211, 255, 255, 0);
    m_Function->m_bColorNodeAdd(249, 100, 100, 0);
    m_Function->m_bColorNodeAdd(250, 0, 0, 0);
    m_Function->m_bColorNodeAdd(255, 0, 0, 0);

    m_Function->m_bAlphaNodeAdd(0, 0);
    m_Function->m_bAlphaNodeAdd(67, 0);
    m_Function->m_bAlphaNodeAdd(68, 10);
    m_Function->m_bAlphaNodeAdd(93, 30);
    m_Function->m_bAlphaNodeAdd(130, 10);
    m_Function->m_bAlphaNodeAdd(131, 0);
    m_Function->m_bAlphaNodeAdd(132, 0);
    m_Function->m_bAlphaNodeAdd(133, 10);
    m_Function->m_bAlphaNodeAdd(211, 200);
    m_Function->m_bAlphaNodeAdd(249, 100);
    m_Function->m_bAlphaNodeAdd(250, 0);
    m_Function->m_bAlphaNodeAdd(255, 0);

//        m_Function->m_bColorNodeAdd(0, 0, 0, 0);
//        m_Function->m_bColorNodeAdd(99, 0, 0, 0);
//        m_Function->m_bColorNodeAdd(100, 0, 255, 0);
//        m_Function->m_bColorNodeAdd(200, 255, 0, 0);
//        m_Function->m_bColorNodeAdd(201, 0, 0, 0);
//        m_Function->m_bColorNodeAdd(255, 0, 0, 0);
//
//        m_Function->m_bAlphaNodeAdd(0, 2);
//        m_Function->m_bAlphaNodeAdd(69, 2);
//        m_Function->m_bAlphaNodeAdd(70, 255);
//        m_Function->m_bAlphaNodeAdd(71, 2);
//        m_Function->m_bAlphaNodeAdd(254, 2);
//        m_Function->m_bAlphaNodeAdd(255, 10);


    // Für Testbilder
//    m_Function->m_bColorNodeAdd(0, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(69, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(70, 255, 0, 0);
//    m_Function->m_bColorNodeAdd(71, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(254, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(255, 0, 255, 0);

//    m_Function->m_bAlphaNodeAdd(0, 2);
//    m_Function->m_bAlphaNodeAdd(69, 2);
//    m_Function->m_bAlphaNodeAdd(70, 255);
//    m_Function->m_bAlphaNodeAdd(71, 2);
//    m_Function->m_bAlphaNodeAdd(254, 2);
//    m_Function->m_bAlphaNodeAdd(255, 10);

    // Für Lineare Test TF
//    m_Function->m_bColorNodeAdd(0, 0, 0, 0);
//    m_Function->m_bColorNodeAdd(50, 50, 50, 50);
//    m_Function->m_bColorNodeAdd(100, 100, 100, 100);
//    m_Function->m_bColorNodeAdd(150, 150, 150, 150);
//    m_Function->m_bColorNodeAdd(200, 200, 200, 200);
//    m_Function->m_bColorNodeAdd(255, 255, 255, 255);

//    m_Function->m_bAlphaNodeAdd(0, 0);
//    m_Function->m_bAlphaNodeAdd(50, 50);
//    m_Function->m_bAlphaNodeAdd(100, 100);
//    m_Function->m_bAlphaNodeAdd(150, 150);
//    m_Function->m_bAlphaNodeAdd(200, 200);
//    m_Function->m_bAlphaNodeAdd(255, 255);
    m_LogFile->m_bAppendMessage(Debug, "(Volume::Volume) OK", FuncDown);
}

Volume::~Volume()
{
    m_LogFile->m_bAppendMessage(Debug, "(Volume::~Volume) Executing", FuncUp);
    if(m_Data) delete m_Data;
    if(m_Function) delete m_Function;
    m_LogFile->m_bAppendMessage(Debug, "(Volume::~Volume) OK", FuncDown);
}

bool Volume::m_bInitializeTex()
{
    return m_bInitializeVolumeTex() && m_bInitializeTransferfunctionTex();
}

bool Volume::m_bInitializeTransferfunctionTex()
{
    return m_Function->m_bBuildColorTableTexture();
}

bool Volume::m_bInitializeVolumeTex()
{
    return m_Data->m_bBuildDataTexture();
}

bool Volume::m_bRenderVolume(iCamera* m_lCam)
{
    m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bRenderVolume) Executing", FuncUp);
    if (m_GlExtInitialised == false)
    {
        m_GlExtInitialised = m_bInitExtensions();
        if (m_GlExtInitialised == false)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bRenderVolume) m_GlExtInitialised == false");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bRenderVolume) FAILED", FuncDown);
            return false;
        }
    }

    if (m_Data->m_bIsTextureInitialized() == false)
    {
        m_LogFile->m_bAppendMessage(Error, "(Volume::m_bRenderVolume) m_Data->m_bIsTextureInitialized() == false");
        m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bRenderVolume) FAILED", FuncDown);
        return false;
    }

    glGetError();
    // makes no sence, because 3d-textture doesnt have a alpha-channel!!
    // maybe we can use this for region making!
    //glEnable(GL_ALPHA_TEST);

	#if SV_USE_SLICING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#endif

    // Using the Multitexturing Extension here, loading the Data-Texture in Texturing-Unit 0
    glextActiveTextureARB(GL_TEXTURE0_ARB);
    glDisable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, m_Data->m_uiGetTextureName());

    // Loading the ColorTable-Texture in Texturing-Unit 1
    glextActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D, m_Function->m_uiGetTextureName());

    // Texture-Color's replaces vertex color defined by glColor3f()
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Set default Vertex-Color to black
    glColor3f(0.0, 0.0, 0.0);

	#ifdef SV_USE_RAYTRACING
	// Bottom
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackLeft().x(), m_RenderCube->m_GetBottomBackLeft().y(), m_RenderCube->m_GetBottomBackLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackRight().x(), m_RenderCube->m_GetBottomBackRight().y(), m_RenderCube->m_GetBottomBackRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontRight().x(), m_RenderCube->m_GetBottomFrontRight().y(), m_RenderCube->m_GetBottomFrontRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontLeft().x(), m_RenderCube->m_GetBottomFrontLeft().y(), m_RenderCube->m_GetBottomFrontLeft().z());
    glEnd();

    // Top
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontLeft().x(), m_RenderCube->m_GetTopFrontLeft().y(), m_RenderCube->m_GetTopFrontLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontRight().x(), m_RenderCube->m_GetTopFrontRight().y(), m_RenderCube->m_GetTopFrontRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackRight().x(), m_RenderCube->m_GetTopBackRight().y(), m_RenderCube->m_GetTopBackRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackLeft().x(), m_RenderCube->m_GetTopBackLeft().y(), m_RenderCube->m_GetTopBackLeft().z());
    glEnd();

    // Front
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontLeft().x(), m_RenderCube->m_GetBottomFrontLeft().y(), m_RenderCube->m_GetBottomFrontLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontRight().x(), m_RenderCube->m_GetBottomFrontRight().y(), m_RenderCube->m_GetBottomFrontRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontRight().x(), m_RenderCube->m_GetTopFrontRight().y(), m_RenderCube->m_GetTopFrontRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontLeft().x(), m_RenderCube->m_GetTopFrontLeft().y(), m_RenderCube->m_GetTopFrontLeft().z());
    glEnd();

    // Back
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackLeft().x(), m_RenderCube->m_GetBottomBackLeft().y(), m_RenderCube->m_GetBottomBackLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackLeft().x(), m_RenderCube->m_GetTopBackLeft().y(), m_RenderCube->m_GetTopBackLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackRight().x(), m_RenderCube->m_GetTopBackRight().y(), m_RenderCube->m_GetTopBackRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackRight().x(), m_RenderCube->m_GetBottomBackRight().y(), m_RenderCube->m_GetBottomBackRight().z());
    glEnd();

    // Right
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontRight().x(), m_RenderCube->m_GetBottomFrontRight().y(), m_RenderCube->m_GetBottomFrontRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackRight().x(), m_RenderCube->m_GetBottomBackRight().y(), m_RenderCube->m_GetBottomBackRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackRight().x(), m_RenderCube->m_GetTopBackRight().y(), m_RenderCube->m_GetTopBackRight().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontRight().x(), m_RenderCube->m_GetTopFrontRight().y(), m_RenderCube->m_GetTopFrontRight().z());
    glEnd();

    // Left
    glBegin(GL_QUADS);
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomFrontLeft().x(), m_RenderCube->m_GetBottomFrontLeft().y(), m_RenderCube->m_GetBottomFrontLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopFrontLeft().x(), m_RenderCube->m_GetTopFrontLeft().y(), m_RenderCube->m_GetTopFrontLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 1.0);
    glVertex3d(m_RenderCube->m_GetTopBackLeft().x(), m_RenderCube->m_GetTopBackLeft().y(), m_RenderCube->m_GetTopBackLeft().z());
    glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, 0.0);
    glVertex3d(m_RenderCube->m_GetBottomBackLeft().x(), m_RenderCube->m_GetBottomBackLeft().y(), m_RenderCube->m_GetBottomBackLeft().z());
    glEnd();
	#endif /* SV_USE_RAYTRACING */

	#ifdef SV_USE_SLICING
    // Setup a cube scaled to the volumedata
    // Attention: Data.Depth = Depht(Count of Images) of the imagestack, this is the Height for the cube!
    //            Data.Height = Height of each Image of the Imagestack, this is the Depth for the cube!
    // THIS IS SO! so dont change!!! (siehe skizze vom 7.7.2011)
    int wid = m_Data->m_GetDataFile()->m_iGetWidth(),
        hei = m_Data->m_GetDataFile()->m_iGetDepth(),
        dep = m_Data->m_GetDataFile()->m_iGetHeight();
    double biggest = 0.0;
    if(wid > biggest) biggest = wid;
    if(hei > biggest) biggest = hei;
    if(dep > biggest) biggest = dep;
    GeoCubeBase cube(QVector3D(0,0,0), 1.0*(hei/biggest), 1.0*(wid/biggest), 1.0*(dep/biggest));

    // Find the cubesite on which the camera is looking
    //       0      1       2       3      4     5
    double unten, vorne, rechts, hinten, links, oben;
    int seite = -1;
    double kleinstespositiveslamda = 100000.0;

    QVector3D p1 = cube.m_GetBottomFrontLeft(), // QVector3D(-0.5, -0.5, 0.5),
              p2 = cube.m_GetBottomBackLeft(), // QVector3D(-0.5, -0.5, -0.5),
              p3 = cube.m_GetBottomFrontRight(), // QVector3D(0.5, -0.5, 0.5),
              gp = m_lCam->m_GetTargetPoint(),    // Kamera Punkt(pos)
              gr = (m_lCam->m_GetPositionPoint() - m_lCam->m_GetTargetPoint());  // Kamera-Vector(pos to target)
    unten = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(unten > 0.0 && unten < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = unten;
        seite = 0;
    }

    p1 = cube.m_GetBottomBackLeft(); // QVector3D(-0.5, -0.5, -0.5);
    p2 = cube.m_GetTopBackLeft(); // QVector3D(-0.5, 0.5, -0.5);
    p3 = cube.m_GetBottomBackRight(); // QVector3D(0.5, -0.5, -0.5);
    vorne = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(vorne > 0.0 && vorne < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = vorne;
        seite = 1;
    }

    p1 = cube.m_GetBottomBackRight(); // QVector3D(0.5, -0.5, -0.5);
    p2 = cube.m_GetTopBackRight(); // QVector3D(0.5, 0.5, -0.5);
    p3 = cube.m_GetBottomFrontRight(); // QVector3D(0.5, -0.5, 0.5);
    rechts = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(rechts > 0.0 && rechts < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = rechts;
        seite = 2;
    }

    p1 = cube.m_GetBottomFrontRight(); // QVector3D(0.5, -0.5, 0.5);
    p2 = cube.m_GetTopFrontRight(); // QVector3D(0.5, 0.5, 0.5);
    p3 = cube.m_GetBottomFrontLeft(); // QVector3D(-0.5, -0.5, 0.5);
    hinten = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(hinten > 0.0 && hinten < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = hinten;
        seite = 3;
    }

    p1 = cube.m_GetBottomFrontLeft(); // QVector3D(-0.5, -0.5, 0.5);
    p2 = cube.m_GetTopFrontLeft(); // QVector3D(-0.5, 0.5, 0.5);
    p3 = cube.m_GetBottomBackLeft(); // QVector3D(-0.5, -0.5, -0.5);
    links = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(links > 0.0 && links < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = links;
        seite = 4;
    }

    p1 = cube.m_GetTopBackLeft(); // QVector3D(-0.5, 0.5, -0.5);
    p2 = cube.m_GetTopFrontLeft(); // QVector3D(-0.5, 0.5, 0.5);
    p3 = cube.m_GetTopBackRight(); // QVector3D(0.5, 0.5, -0.5);
    oben = m_bGeradeSchneidetEbeneVektorForm(p1,p2,p3,gp, gr);
    if(oben > 0.0 && oben < kleinstespositiveslamda)
    {
        kleinstespositiveslamda = oben;
        seite = 5;
    }

    if(seite == -1) m_LogFile->m_bAppendMessage(Error, "seite == -1 -> unten=" + QString::number(unten) +
                                                " vorne=" + QString::number(vorne) +
                                                " rechts=" + QString::number(rechts) +
                                                " hinten=" + QString::number(hinten) +
                                                " links=" + QString::number(links) +
                                                " oben=" + QString::number(oben));

    // Render the slicestack in the right direction
    // 0:0:0 von der textur ist immer bei -0.5:-0.5:-0.5
    GLdouble high, texhigh;
    double Count = 500;
    // zeichnet von -y nach +y

    if(seite == 5) // 5 = oben
    {
        high = -1.0*cube.m_dGetHeight()/2.0;
        texhigh = 0.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, texhigh);
            glVertex3d(cube.m_GetBottomBackLeft().x(), high, cube.m_GetBottomBackLeft().z()); // (-0.5, high, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, texhigh);
            glVertex3d(cube.m_GetBottomBackRight().x(), high, cube.m_GetBottomBackRight().z()); // (0.5, high, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, texhigh);
            glVertex3d(cube.m_GetBottomFrontRight().x(), high, cube.m_GetBottomFrontRight().z()); // (0.5, high, 0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, texhigh);
            glVertex3d(cube.m_GetBottomFrontLeft().x(), high, cube.m_GetBottomFrontLeft().z()); // (-0.5, high, 0.5);
            glEnd();

            high += cube.m_dGetHeight()/Count;
            texhigh += 1.0/Count;
        }
    }

    // zeichnet von +y nach -y
    if(seite == 0) // 0 = unten
    {
        high = 1.0*cube.m_dGetHeight()/2.0;
        texhigh = 1.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 0.0, texhigh);
            glVertex3d(cube.m_GetBottomBackLeft().x(), high, cube.m_GetBottomBackLeft().z()); // (-0.5, high, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 0.0, texhigh);
            glVertex3d(cube.m_GetBottomBackRight().x(), high, cube.m_GetBottomBackRight().z()); // (0.5, high, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, 1.0, texhigh);
            glVertex3d(cube.m_GetBottomFrontRight().x(), high, cube.m_GetBottomFrontRight().z()); // (0.5, high, 0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, 1.0, texhigh);
            glVertex3d(cube.m_GetBottomFrontLeft().x(), high, cube.m_GetBottomFrontLeft().z()); // (-0.5, high, 0.5);
            glEnd();

            high -= cube.m_dGetHeight()/Count;
            texhigh -= 1.0/Count;
        }
    }

    // zeichnet von -x nach +x
    if(seite == 2) // 2 = rechts
    {
        high = -1.0*cube.m_dGetWidth()/2.0;
        texhigh = 0.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 0.0, 0.0);
            glVertex3d(high, cube.m_GetBottomBackLeft().y(), cube.m_GetBottomBackLeft().z()); // (high, -0.5, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 0.0, 1.0);
            glVertex3d(high, cube.m_GetTopBackLeft().y(), cube.m_GetTopBackLeft().z()); // (high, 0.5, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 1.0, 1.0);
            glVertex3d(high, cube.m_GetTopFrontLeft().y(), cube.m_GetTopFrontLeft().z()); // (high, 0.5, 0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 1.0, 0.0);
            glVertex3d(high, cube.m_GetBottomFrontLeft().y(), cube.m_GetBottomFrontLeft().z()); // (high, -0.5, 0.5);
            glEnd();

            high += cube.m_dGetWidth()/Count;
            texhigh += 1.0/Count;
        }
    }

    // zeichnet von +x nach -x
    if(seite == 4) // 4 = links
    {
        high = 1.0*cube.m_dGetWidth()/2.0;
        texhigh = 1.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 0.0, 0.0);
            glVertex3d(high, cube.m_GetBottomBackLeft().y(), cube.m_GetBottomBackLeft().z()); // (high, -0.5, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 0.0, 1.0);
            glVertex3d(high, cube.m_GetTopBackLeft().y(), cube.m_GetTopBackLeft().z()); // (high, 0.5, -0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 1.0, 1.0);
            glVertex3d(high, cube.m_GetTopFrontLeft().y(), cube.m_GetTopFrontLeft().z()); // (high, 0.5, 0.5);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, texhigh, 1.0, 0.0);
            glVertex3d(high, cube.m_GetBottomFrontLeft().y(), cube.m_GetBottomFrontLeft().z()); // (high, -0.5, 0.5);
            glEnd();

            high -= cube.m_dGetWidth()/Count;
            texhigh -= 1.0/Count;
        }
    }

    // zeichnet von -z nach +z
    if(seite == 3) // 3 = hinten
    {
        high = 1.0*cube.m_dGetDepth()/2.0;
        texhigh = 0.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, texhigh, 0.0);
            glVertex3d(cube.m_GetBottomBackLeft().x(), cube.m_GetBottomBackLeft().y(), high); // (-0.5, -0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, texhigh, 1.0);
            glVertex3d(cube.m_GetTopBackLeft().x(), cube.m_GetTopBackLeft().y(), high); // (-0.5, 0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, texhigh, 1.0);
            glVertex3d(cube.m_GetTopBackRight().x(), cube.m_GetTopBackRight().y(), high); // (0.5, 0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, texhigh, 0.0);
            glVertex3d(cube.m_GetBottomBackRight().x(), cube.m_GetBottomBackRight().y(), high); // (0.5, -0.5, high);
            glEnd();

            high -= cube.m_dGetDepth()/Count;
            texhigh += 1.0/Count;
        }
    }

    // zeichnet von +z nach -z
    if(seite == 1) // 1 = vorne
    {
        high = -1.0*cube.m_dGetDepth()/2.0;
        texhigh = 1.0;
        for(GLdouble i=0; i<=Count; i++)
        {
            glBegin(GL_QUADS);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, texhigh, 0.0);
            glVertex3d(cube.m_GetBottomBackLeft().x(), cube.m_GetBottomBackLeft().y(), high); // (-0.5, -0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 0.0, texhigh, 1.0);
            glVertex3d(cube.m_GetTopBackLeft().x(), cube.m_GetTopBackLeft().y(), high); // (-0.5, 0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, texhigh, 1.0);
            glVertex3d(cube.m_GetTopBackRight().x(), cube.m_GetTopBackRight().y(), high); // (0.5, 0.5, high);
            glextMultiTexCoord3fARB(GL_TEXTURE0_ARB, 1.0, texhigh, 0.0);
            glVertex3d(cube.m_GetBottomBackRight().x(), cube.m_GetBottomBackRight().y(), high); // (0.5, -0.5, high);
            glEnd();

            high += cube.m_dGetDepth()/Count;
            texhigh -= 1.0/Count;
        }
    }
	#endif /* SV_USE_SLICING */

    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(Volume::m_bRenderVolume) Got glError on entering this function (Error-Code " + QString::number(err) + ")");
        m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bRenderVolume) FAILED", FuncDown);
        return false;
    }

    m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bRenderVolume) OK", FuncDown);
    return true;
}

bool Volume::m_bUpdateColorTableTex()
{
    m_Function->m_bBuildColorTableTexture();
    return true;
}

int Volume::m_iGetColorTableTexName()
{
    return m_Function->m_uiGetTextureName();
}

int Volume::m_iGetVolumeDataTexName()
{
    return m_Data->m_uiGetTextureName();
}

bool Volume::m_bInitExtensions()
{
    m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) Executing", FuncUp);
    #ifdef _WIN32
        // Multi-Texturing (under Windows)

        // NEEDS GL_ARB_multitexture EXTENSION
        glextMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
        if (glextMultiTexCoord1fARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glMultiTexCoord1fARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glMultiTexCoord1fARB') .. OK");

        // NEEDS GL_ARB_multitexture EXTENSION
        glextMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
        if (glextMultiTexCoord3fARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glMultiTexCoord3fARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glMultiTexCoord3fARB') .. OK");

        // NEEDS GL_ARB_multitexture EXTENSION
        glextActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
        if (glextActiveTextureARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glActiveTextureARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glActiveTextureARB') .. OK");

        // NEEDS GL_ARB_multitexture EXTENSION
        glextClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
        if (glextClientActiveTextureARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glClientActiveTextureARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> wglGetProcAddress('glClientActiveTextureARB') .. OK");
    #else
        // Multi-Texturing (under Linux)
        glextMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) glXGetProcAddress((GLubyte*)"glMultiTexCoord1fARB");
        if (glextMultiTexCoord1fARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glMultiTexCoord1fARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glMultiTexCoord1fARB') .. OK");

        glextMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) glXGetProcAddress((GLubyte*)"glMultiTexCoord3fARB");
        if (glextMultiTexCoord3fARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glMultiTexCoord3fARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glMultiTexCoord3fARB') .. OK");

        glextActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) glXGetProcAddress((GLubyte*)"glActiveTextureARB");
        if (glextActiveTextureARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glActiveTextureARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glActiveTextureARB') .. OK");

        glextClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) glXGetProcAddress((GLubyte*)"glClientActiveTextureARB");
        if (glextClientActiveTextureARB == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glClientActiveTextureARB') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) FAILED", FuncDown);
            return FALSE;
        }
        else
            m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glClientActiveTextureARB') .. OK");
    #endif

    m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bInitExtensions) OK", FuncDown);
    return true;
}
iTransferFunction* Volume::m_GetTransferfuction()
{
    return m_Function;
}

bool Volume::m_bSetDataFile(TexArray3D* file)
{
    m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bSetDataFile) Executing", FuncUp);
    if(m_Data->m_bSetDataFile(file))
    {
		#ifdef SV_USE_RAYTRACING
        // Setup a cube scaled to the volumedata
        // Attention: Data.Depth = Depht(Count of Images) of the imagestack, this is the Height for the cube!
        //            Data.Height = Height of each Image of the Imagestack, this is the Depth for the cube!
        // THIS IS SO! so dont change!!! (siehe skizze vom 7.7.2011)
//        int wid = m_Data->m_GetDataFile()->m_iGetWidth(),
//            hei = m_Data->m_GetDataFile()->m_iGetDepth(),
//            dep = m_Data->m_GetDataFile()->m_iGetHeight();

        // 25.11.11 PM Maybe that was crap, on raytracing this just looks wrong ;)
        int wid = m_Data->m_GetDataFile()->m_iGetWidth(),
            hei = m_Data->m_GetDataFile()->m_iGetHeight(),
            dep = m_Data->m_GetDataFile()->m_iGetDepth();

        double biggest = 0.0;
        if(wid > biggest) biggest = wid;
        if(hei > biggest) biggest = hei;
        if(dep > biggest) biggest = dep;
        m_RenderCube = new GeoCubeBase(QVector3D(0,0,0), 1.0*(hei/biggest), 1.0*(wid/biggest), 1.0*(dep/biggest));
		#endif /* SV_USE_RAYTRACING */
        
		emit m_vVolumeDataChanged();
        m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bSetDataFile) OK", FuncDown);
        return true;
    }
    else
    {
        m_LogFile->m_bAppendMessage(Debug, "(Volume::m_bSetDataFile) FAILED", FuncDown);
        return false;
    }
}

QList<double>* Volume::m_GetHistogram()
{
    return m_Data->m_GetHistogram();
}

bool Volume::m_bHasData()
{
    return m_Data->m_bHasData();
}

double Volume::m_bGeradeSchneidetEbeneVektorForm(QVector3D ebenep1, QVector3D ebenep2, QVector3D ebenep3, QVector3D geradepunkt, QVector3D geraderichtungsvector)
{
    QVector3D n;
    n = QVector3D::crossProduct((ebenep2-ebenep1), (ebenep3-ebenep1));
    if((n*geraderichtungsvector).isNull() == true) return -1;
    double zaehler = QVector3D::dotProduct(n,ebenep1-geradepunkt);
    double nenner = QVector3D::dotProduct(n, geraderichtungsvector);

    if(nenner == 0.0) return -1; // gerade and vektor are parallel!
    return (zaehler/nenner);
}
