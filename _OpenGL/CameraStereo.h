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

#ifndef CAMERASTEREO_H
#define CAMERASTEREO_H

#include <math.h>
#include <QGLWidget>
#include "_Misc/LogFile.h"
#include "_OpenGL/iCamera.h"
#include <QVector3D>
#include "_OpenGL/CameraSimple.h"

class CameraStereo : public iCamera
{
public:
    CameraStereo(iCamera* basiscam, StereoSettings* stereosettings);
    CameraStereo(QGLWidget *parent, QVector3D pos, QVector3D tar, QVector3D upv, int width, int height, double dnear, double dfar, double fieldofview, StereoSettings* stereosettings);
    virtual bool m_bCalcFrustum(int width, int height);
    virtual bool m_bMousePressHandler(QMouseEvent *event);
    virtual bool m_bMouseMoveHandler(QMouseEvent *event);
    virtual bool m_bMouseWheelHandler(QWheelEvent *event);
    virtual bool m_bKeyPressHandler(QKeyEvent *event);
    virtual bool m_glbSetupFrustum();
    virtual bool m_glbSetupLookAt();
    virtual QVector3D m_GetPositionPoint() const;
    virtual QVector3D m_GetTargetPoint() const;
    virtual QVector3D m_GetUpVector() const;
    virtual bool m_bSetNear(double dnear);
    virtual bool m_bSetFar(double dfar);
    virtual bool m_bSetFieldOfView(double fov);
    virtual double m_dGetNear() const;
    virtual double m_dGetFar() const;
    virtual double m_dGetFieldOfView() const;
    virtual bool m_glbUpdateParentOpenGLWidget() const;

    bool m_bCalcEyePositions();
    bool m_glbSetupLookAtRightEye();
    bool m_glbSetupLookAtLeftEye();
    iCamera* m_GetBaseCamera();

private:
    LogFile* m_LogFile;
    iCamera* m_BasisCamera;
    QVector3D m_RightEyePositionPoint, m_RightEyeTargetPoint, m_RightEyeUpVector;
    QVector3D m_LeftEyePositionPoint, m_LeftEyeTargetPoint, m_LeftEyeUpVector;
    Frustum m_Frustum;
    QPoint m_LastMousePos;
    double m_dPlanNear, m_dPlanFar, m_dFieldOfView, m_dZoomFactor;
    int m_iOldWidth, m_iOldHeight;

    // stereo fields
    bool m_bLeftEye;
    StereoSettings* m_StereoSettings;

    virtual bool m_bCalcFrustum();
};

#endif // CAMERASTEREO_H
