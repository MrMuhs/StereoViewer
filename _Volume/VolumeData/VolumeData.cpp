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

#include "_Volume/VolumeData/VolumeData.h"

VolumeData::VolumeData()
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::VolumeData) Executing", FuncUp);
    m_GlExtInitialised = false;
    m_bTexIsInitialized = false;
    m_DataFile = 0;
    m_uiTexName = NULL;
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::VolumeData) OK", FuncDown);
}

VolumeData::~VolumeData()
{
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::~VolumeData) Executing", FuncUp);
    if(m_uiTexName)
    {
        glDeleteTextures(1, &m_uiTexName);
        m_bTexIsInitialized = false;
    }
    if(m_DataFile) delete m_DataFile;
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::~VolumeData) OK", FuncDown);
}

bool VolumeData::m_bBuildDataTexture()
{
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) Executing", FuncUp);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) Got glError on entering this function (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) No glErrors on entering function");

    if (m_GlExtInitialised == false)
    {
        m_GlExtInitialised = m_bInitExtensions();
        if (m_GlExtInitialised == false)
        {
            m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) -> m_GlExtInitialised == false");
            m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) FAILED", FuncDown);
            return false;
        }
    }

    if(m_bHasData() == false)
    {
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) -> m_bHasData() == false");
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) FAILED", FuncDown);
        return false;
    }

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) Got initial glError (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) No initial glErrors");

    // request 1 texture name from OpenGL, delete old before
    if(m_uiTexName != NULL)
    {
        glDeleteTextures(1, &m_uiTexName);
        glGenTextures(1, &m_uiTexName);
    }
    else
        glGenTextures(1, &m_uiTexName);

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) glGenTextures() got glError (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) No glErrors on glGenTextures()");

    // tell OpenGL we're going to be setting up the texture name it gave us
    glBindTexture(GL_TEXTURE_3D, m_uiTexName);
    // when this texture needs to be shrunk to fit on small polygons, use linear interpolation of the texels to determine the color
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // when this texture needs to be magnified to fit on a big polygon, use linear interpolation of the texels to determine the color
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // we want the texture to repeat over the S axis, so if we specify coordinates out of range we still get textured.
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // same as above for T axis
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_REPEAT
    // same as above for R axis
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) Got glError on setting Texture-Parameters (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) No glErrors setting Texture-Parameters");

    // experimenting here with 16bit grayscale texture, but on 512³*2 bytes texture is too heavy for most graficcards atm.
    // this is limited by the os in cases, have a look at(and links into it):
    // http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=290743

//    if(m_DataFile->m_iGetBytesPerTexel() == 1)
    glextTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_DataFile->m_iGetWidth(), m_DataFile->m_iGetHeight(),
                    m_DataFile->m_iGetDepth(), 0, GL_RED, GL_UNSIGNED_BYTE, m_DataFile->m_ucGetTextureData());
//    if(m_DataFile->m_iGetBytesPerTexel() == 2)
//        glextTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, m_DataFile->m_iGetWidth(), m_DataFile->m_iGetHeight(),
//                        m_DataFile->m_iGetDepth(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, m_DataFile->m_ucGetTextureData());

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bBuildDataTexture) Got glError on glextTexImage3D(..) (Error-Code " + QString::number(err) + ")");
        m_bTexIsInitialized = false;
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) FAILED", FuncDown);
        return false;
    }
    else
    {
        m_bTexIsInitialized = true;
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bBuildDataTexture) OK", FuncDown);
        return true;
    }
}

unsigned int VolumeData::m_uiGetTextureName()
{
    return m_uiTexName;
}

bool VolumeData::m_bInitExtensions()
{
    m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) Executing", FuncUp);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bInitExtensions) Got glError on entering this function (Error-Code " + QString::number(err) + ")");
        err = GL_NO_ERROR;
    }
    else
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) No glErrors on entering function");

    #ifdef _WIN32
        // 3D-Texture Extension (on a Windows)

        // NEEDS GL_ARB_multitexture EXTENSION and GL Version >= 1.2
        glextTexImage3D = (PFNGLTEXIMAGE3DPROC) wglGetProcAddress("glTexImage3D");
        if (glextTexImage3D == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bInitExtensions) -> wglGetProcAddress('glTexImage3D') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) FAILED", FuncDown);
            return false;
        }
        else
        {
            m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) -> wglGetProcAddress('glTexImage3D') .. OK");
        }
    #else
        // 3D-Texture Extension (on a Linux)
        glextTexImage3D = (PFNGLTEXIMAGE3DPROC) glXGetProcAddress((GLubyte*)"glTexImage3D");
        if (glextTexImage3D == NULL)
        {
            m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glTexImage3D') == NULL");
            m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) FAILED", FuncDown);
            return false;
        }
        else
        {
            m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) -> glXGetProcAddress((GLubyte*)'glTexImage3D') .. OK");
        }
    #endif

    err = glGetError();
    if(err != GL_NO_ERROR)
    {
        m_LogFile->m_bAppendMessage(Error, "(VolumeData::m_bInitExtensions) Got glError after InitExtensions (Error-Code " + QString::number(err) + ")");
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) FAILED", FuncDown);
        return false;
    }
    else
    {
        // Everyting was ok!
        m_LogFile->m_bAppendMessage(Debug, "(VolumeData::m_bInitExtensions) OK", FuncDown);
        return true;
    }
}

bool VolumeData::m_bHasData()
{
    if(m_DataFile) return true;
    return false;
}

bool VolumeData::m_bIsTextureInitialized()
{
    return m_bTexIsInitialized;
}

bool VolumeData::m_bSetDataFile(TexArray3D* file)
{
    m_bTexIsInitialized = false;
    m_DataFile = file;
    m_bBuildDataTexture();
    return true;
}

TexArray3D* VolumeData::m_GetDataFile()
{
    return m_DataFile;
}

QList<double>* VolumeData::m_GetHistogram()
{
    if (m_bTexIsInitialized)
        return m_DataFile->m_GetHistogram();
    else
        return 0;
}
