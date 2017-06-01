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

#include <QDebug>
#include "CameraSimple.h"
#include "gluLookAtReplacement.h"

CameraSimple::CameraSimple(QGLWidget *parent, QVector3D pos, QVector3D tar, QVector3D upv, int width, int height, double dnear, double dfar, double fieldofview)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    // Handle Inputs
    m_ParentOpenGLWidget = parent;
    m_PositionPoint = pos;
    m_TargetPoint = tar;
    m_UpVector = upv;
    m_dPlanNear = dnear;
    m_dPlanFar = dfar;
    m_dFieldOfView = fieldofview;

    // Init other class-fields
    m_iOldWidth = width;
    m_iOldHeight = height;
    m_dZoomFactor = 1.0;
    m_bCalcFrustum();
}

CameraSimple::~CameraSimple()
{

}

bool CameraSimple::m_bCalcFrustum()
{
    if (m_iOldWidth == 0 && m_iOldHeight == 0) return false;
    return m_bCalcFrustum(m_iOldWidth, m_iOldHeight);
}

bool CameraSimple::m_bCalcFrustum(int width, int height)
{
    m_iOldWidth = width;
    m_iOldHeight = height;

    // generate a perspectiv frustum with zoom faktor
    double top = m_dPlanNear * tan(m_dFieldOfView * PI /360) * m_dZoomFactor;
    m_Frustum.Top = top;
    m_Frustum.Bottom = (-top);
    m_Frustum.Left = (-top * (double)width/(double)height);
    m_Frustum.Right = (top * (double)width/(double)height);

    m_ParentOpenGLWidget->updateGL();
    return true;
}

bool CameraSimple::m_bMousePressHandler(QMouseEvent *event)
{
    m_LastMousePos = event->pos();
    return true;
}

bool CameraSimple::m_bMouseMoveHandler(QMouseEvent *event)
{
    double dx = event->x() - m_LastMousePos.x();
    double dy = event->y() - m_LastMousePos.y();
    dx *= 0.05;
    dy *= 0.05;
    double degreex = 0, degreey = 0, degreez = 0;

    if (event->buttons() & Qt::LeftButton)
    {
        if(dx != 0)
            degreey = (dx) / (2*PI);
        if(dy != 0)
            degreex = (dy) / (2*PI);

        QVector3D PosRot = m_PositionPoint - m_TargetPoint;

        // Rot Z-Achse
        QVector3D oldvector = PosRot;

        // Rot Y-Achse
        oldvector = PosRot;
        PosRot.setX( oldvector.x()*cos(degreey) + oldvector.z()*sin(degreey));
        PosRot.setY( oldvector.y());
        PosRot.setZ(-oldvector.x()*sin(degreey) + oldvector.z()*cos(degreey));

        // Rot X-Achse
        oldvector = PosRot;
        PosRot.setX(oldvector.x());
        PosRot.setY(oldvector.y()*cos(degreex) - oldvector.z()*sin(degreex));
        PosRot.setZ(oldvector.y()*sin(degreex) + oldvector.z()*cos(degreex));

        m_PositionPoint = PosRot + m_TargetPoint;



        PosRot = m_UpVector - m_TargetPoint;

        // Rot Y-Achse
        oldvector = PosRot;
        PosRot.setX( oldvector.x()*cos(degreey) + oldvector.z()*sin(degreey));
        PosRot.setY( oldvector.y());
        PosRot.setZ(-oldvector.x()*sin(degreey) + oldvector.z()*cos(degreey));

        // Rot X-Achse
        oldvector = PosRot;
        PosRot.setX(oldvector.x());
        PosRot.setY(oldvector.y()*cos(degreex) - oldvector.z()*sin(degreex));
        PosRot.setZ(oldvector.y()*sin(degreex) + oldvector.z()*cos(degreex));

        m_UpVector = PosRot + m_TargetPoint;
    }
    else if (event->buttons() & Qt::RightButton)
    {
        if(dx != 0)
            degreey = (dx) / (2*PI);
        if(dy != 0)
            degreez = (dy) / (2*PI);

        QVector3D PosRot = m_PositionPoint - m_TargetPoint;

        // Rot Z-Achse
        QVector3D oldvector = PosRot;
        PosRot.setX(oldvector.x()*cos(degreez) - oldvector.y()*sin(degreez));
        PosRot.setY(oldvector.x()*sin(degreez) + oldvector.y()*cos(degreez));
        PosRot.setZ(oldvector.z());

        // Rot Y-Achse
        oldvector = PosRot;
        PosRot.setX( oldvector.x()*cos(degreey) + oldvector.z()*sin(degreey));
        PosRot.setY( oldvector.y());
        PosRot.setZ(-oldvector.x()*sin(degreey) + oldvector.z()*cos(degreey));

        m_PositionPoint = PosRot + m_TargetPoint;



        PosRot = m_UpVector - m_TargetPoint;

        // Rot Z-Achse
        oldvector = PosRot;
        PosRot.setX(oldvector.x()*cos(degreez) - oldvector.y()*sin(degreez));
        PosRot.setY(oldvector.x()*sin(degreez) + oldvector.y()*cos(degreez));
        PosRot.setZ(oldvector.z());

        // Rot Y-Achse
        oldvector = PosRot;
        PosRot.setX( oldvector.x()*cos(degreey) + oldvector.z()*sin(degreey));
        PosRot.setY( oldvector.y());
        PosRot.setZ(-oldvector.x()*sin(degreey) + oldvector.z()*cos(degreey));

        m_UpVector = PosRot + m_TargetPoint;
    }
    else if (event->buttons() & Qt::MidButton)
    {

    }

    m_LastMousePos = event->pos();
    m_ParentOpenGLWidget->updateGL();

    qDebug() << "Pos=(" << m_PositionPoint.x() << m_PositionPoint.y() << m_PositionPoint.z()
             << "Tar=" << m_TargetPoint.x() << m_TargetPoint.y() << m_TargetPoint.z()
             << "Up =" << m_UpVector.x() << m_UpVector.y() << m_UpVector.z();
    return true;
}

bool CameraSimple::m_bMouseWheelHandler(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    m_dZoomFactor = m_dZoomFactor + (numSteps) * ZoomFactorIncerment;
    m_bCalcFrustum();
    return true;
}

bool CameraSimple::m_bKeyPressHandler(QKeyEvent *event)
{
    return true;
}

bool CameraSimple::m_glbSetupFrustum()
{
    glFrustum(m_Frustum.Left, m_Frustum.Right, m_Frustum.Bottom, m_Frustum.Top, m_dPlanNear, m_dPlanFar);
    return true;
}

bool CameraSimple::m_glbSetupLookAt()
{
    gluLookAt(m_PositionPoint.x(), m_PositionPoint.y(), m_PositionPoint.z(),
              m_TargetPoint.x(), m_TargetPoint.y(), m_TargetPoint.z(),
              m_UpVector.x(), m_UpVector.y(), m_UpVector.z());
    return true;
}

QVector3D CameraSimple::m_GetPositionPoint() const
{
    return m_PositionPoint;
}

QVector3D CameraSimple::m_GetTargetPoint() const
{
    return m_TargetPoint;
}

QVector3D CameraSimple::m_GetUpVector() const
{
    return m_UpVector;
}

bool CameraSimple::m_bSetNear(double dnear)
{
    if(dnear >= m_dPlanFar) return false;
    m_dPlanNear = dnear;
    // Frustum depends on the NearPlan
    m_bCalcFrustum();
    return true;
}

bool CameraSimple::m_bSetFar(double dfar)
{
    if(dfar <= m_dPlanNear) return false;
    m_dPlanFar = dfar;
    m_ParentOpenGLWidget->updateGL();
    return true;
}

bool CameraSimple::m_bSetFieldOfView(double fov)
{
    if(fov <= 0 || fov > 360) return false;
    m_dFieldOfView = fov;
    // Frustum depends on the FieldOfView
    m_bCalcFrustum();
    return true;
}

double CameraSimple::m_dGetNear() const
{
    return m_dPlanNear;
}

double CameraSimple::m_dGetFar() const
{
    return m_dPlanFar;
}

double CameraSimple::m_dGetFieldOfView() const
{
    return m_dFieldOfView;
}

bool CameraSimple::m_glbUpdateParentOpenGLWidget() const
{
    m_ParentOpenGLWidget->updateGL();
    return true;
}
