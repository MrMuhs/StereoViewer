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

#include "_OpenGL/CameraStereo.h"

CameraStereo::CameraStereo(iCamera* basiscam, StereoSettings* stereosettings)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    m_StereoSettings = stereosettings;
    m_BasisCamera = basiscam;
    m_RightEyePositionPoint = m_BasisCamera->m_GetPositionPoint();
    m_RightEyeTargetPoint = m_BasisCamera->m_GetTargetPoint();
    m_RightEyeUpVector = m_BasisCamera->m_GetUpVector();
    m_LeftEyePositionPoint = m_BasisCamera->m_GetPositionPoint();
    m_LeftEyeTargetPoint = m_BasisCamera->m_GetTargetPoint();
    m_LeftEyeUpVector = m_BasisCamera->m_GetUpVector();
    m_bCalcEyePositions();
    m_bLeftEye = true; // starting with lefteye pos, take care of in render function!!!
}

CameraStereo::CameraStereo(QGLWidget *parent, QVector3D pos, QVector3D tar, QVector3D upv, int width, int height, double dnear, double dfar, double fieldofview, StereoSettings* stereosettings)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    m_StereoSettings = stereosettings;
    m_BasisCamera = new CameraSimple(parent, pos, tar, upv, width, height, dnear, dfar, fieldofview);
    m_RightEyePositionPoint = m_BasisCamera->m_GetPositionPoint();
    m_RightEyeTargetPoint = m_BasisCamera->m_GetTargetPoint();
    m_RightEyeUpVector = m_BasisCamera->m_GetUpVector();
    m_LeftEyePositionPoint = m_BasisCamera->m_GetPositionPoint();
    m_LeftEyeTargetPoint = m_BasisCamera->m_GetTargetPoint();
    m_LeftEyeUpVector = m_BasisCamera->m_GetUpVector();
    m_bCalcEyePositions();
    m_bLeftEye = true; // starting with lefteye pos, take care of in render function!!!
}

// Methodes from iCamera-Interface ->
bool CameraStereo::m_bCalcFrustum(int width, int height)
{
    return m_BasisCamera->m_bCalcFrustum(width, height);
}

bool CameraStereo::m_bCalcFrustum()
{
    if (m_iOldWidth == 0 && m_iOldHeight == 0) return false;
    return m_bCalcFrustum(m_iOldWidth, m_iOldHeight);
}

bool CameraStereo::m_bMousePressHandler(QMouseEvent *event)
{
    return m_BasisCamera->m_bMousePressHandler(event) && m_bCalcEyePositions();
}

bool CameraStereo::m_bMouseMoveHandler(QMouseEvent *event)
{
    return m_BasisCamera->m_bMouseMoveHandler(event) && m_bCalcEyePositions();
}

bool CameraStereo::m_bMouseWheelHandler(QWheelEvent *event)
{
    return m_BasisCamera->m_bMouseWheelHandler(event) && m_bCalcEyePositions();
}

bool CameraStereo::m_bKeyPressHandler(QKeyEvent *event)
{
    return m_BasisCamera->m_bKeyPressHandler(event) && m_bCalcEyePositions();
}

bool CameraStereo::m_glbSetupFrustum()
{
    return m_BasisCamera->m_glbSetupFrustum();
}

bool CameraStereo::m_glbSetupLookAt()
{
    // this switches the lookat mathodes within the 2 calls of this methode
    // THIS FUNKTION IS OBSOLETE (PM 06.05.2011 not in use anymore, just keep it for iCamera interface)
//    if(m_bLeftEye == true)
//    {
//        m_bLeftEye = false;
//        return this->m_glbSetupLookAtLeftEye();
//    }
//    else
//    {
//        m_bLeftEye = true;
//        return this->m_glbSetupLookAtRightEye();
//    }
   return false;
}

QVector3D CameraStereo::m_GetPositionPoint() const
{
    return m_BasisCamera->m_GetPositionPoint();
}

QVector3D CameraStereo::m_GetTargetPoint() const
{
    return m_BasisCamera->m_GetTargetPoint();
}

QVector3D CameraStereo::m_GetUpVector() const
{
    return m_BasisCamera->m_GetUpVector();
}

bool CameraStereo::m_bSetNear(double dnear)
{
    return m_BasisCamera->m_bSetNear(dnear);
}

bool CameraStereo::m_bSetFar(double dfar)
{
    return m_BasisCamera->m_bSetFar(dfar);
}

bool CameraStereo::m_bSetFieldOfView(double fov)
{
    return m_BasisCamera->m_bSetFieldOfView(fov);
}

double CameraStereo::m_dGetNear() const
{
    return m_BasisCamera->m_dGetNear();
}

double CameraStereo::m_dGetFar() const
{
    return m_BasisCamera->m_dGetFar();
}

double CameraStereo::m_dGetFieldOfView() const
{
    return m_BasisCamera->m_dGetFieldOfView();
}

// <- End

bool CameraStereo::m_glbSetupLookAtRightEye()
{
    gluLookAt(m_RightEyePositionPoint.x(), m_RightEyePositionPoint.y(), m_RightEyePositionPoint.z(),
              m_RightEyeTargetPoint.x(), m_RightEyeTargetPoint.y(), m_RightEyeTargetPoint.z(),
              m_RightEyeUpVector.x(), m_RightEyeUpVector.y(), m_RightEyeUpVector.z());
    return true;
}

bool CameraStereo::m_glbSetupLookAtLeftEye()
{
    gluLookAt(m_LeftEyePositionPoint.x(), m_LeftEyePositionPoint.y(), m_LeftEyePositionPoint.z(),
              m_LeftEyeTargetPoint.x(), m_LeftEyeTargetPoint.y(), m_LeftEyeTargetPoint.z(),
              m_LeftEyeUpVector.x(), m_LeftEyeUpVector.y(), m_LeftEyeUpVector.z());
    return true;
}

iCamera* CameraStereo:: m_GetBaseCamera()
{
    return m_BasisCamera;
}

bool CameraStereo::m_bCalcEyePositions()
{
    QVector3D pos = m_BasisCamera->m_GetPositionPoint(), tar = m_BasisCamera->m_GetTargetPoint(), up = m_BasisCamera->m_GetUpVector();
    QVector3D CrossProd = QVector3D::crossProduct(up, pos - tar);
    CrossProd.normalize();
    CrossProd *= m_StereoSettings->m_dStereoBasis;

    QVector3D PosToTarget = tar - pos;
    PosToTarget.normalize();
    PosToTarget *= m_StereoSettings->m_dFernpunktweite;
    PosToTarget -= (tar - pos);

    m_RightEyePositionPoint = pos - (CrossProd/2.0);
    m_RightEyeTargetPoint = tar + PosToTarget;
    m_RightEyeUpVector = up;
    m_LeftEyePositionPoint = pos + (CrossProd/2.0);
    m_LeftEyeTargetPoint = tar + PosToTarget;
    m_LeftEyeUpVector = up;

    m_glbUpdateParentOpenGLWidget();
    return true;
}

bool CameraStereo::m_glbUpdateParentOpenGLWidget() const
{
    m_BasisCamera->m_glbUpdateParentOpenGLWidget();
    return true;
}
