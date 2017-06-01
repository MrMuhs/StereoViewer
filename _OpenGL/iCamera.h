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

#ifndef ICAMERA_H
#define ICAMERA_H

#include <QMouseEvent>
#include "_Misc/_Defines.h"

class iCamera
{
public:
    virtual ~iCamera() {}
    virtual bool m_bCalcFrustum(int width, int height) = 0;
    virtual bool m_bMousePressHandler(QMouseEvent *event) = 0;
    virtual bool m_bMouseMoveHandler(QMouseEvent *event) = 0;
    virtual bool m_bMouseWheelHandler(QWheelEvent *event) = 0;
    virtual bool m_bKeyPressHandler(QKeyEvent *event) = 0;
    virtual bool m_glbSetupFrustum() = 0;
    virtual bool m_glbSetupLookAt() = 0;
    virtual QVector3D m_GetPositionPoint() const = 0;
    virtual QVector3D m_GetTargetPoint() const = 0;
    virtual QVector3D m_GetUpVector() const = 0;
    virtual bool m_bSetNear(double dnear) = 0;
    virtual bool m_bSetFar(double dfar) = 0;
    virtual bool m_bSetFieldOfView(double fov) = 0;
    virtual double m_dGetNear() const = 0;
    virtual double m_dGetFar() const = 0;
    virtual double m_dGetFieldOfView() const = 0;
    virtual bool m_glbUpdateParentOpenGLWidget() const = 0;
private:
    virtual bool m_bCalcFrustum() = 0; // need to be used interal, if far/near changes
};

#endif // ICAMERA_H
