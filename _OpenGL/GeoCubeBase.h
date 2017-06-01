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

#ifndef GEOCUBEBASE_H
#define GEOCUBEBASE_H

#include <QVector3D>

class GeoCubeBase
{
public:
    GeoCubeBase(QVector3D center, double height, double width, double depth);
    QVector3D m_GetWorldCenter() const { return m_WorldCenter; }
    // relativ zum Center der Welt
    QVector3D m_GetTopFrontRight() const { return m_TopFrontRight; }
    QVector3D m_GetTopFrontLeft() const { return m_TopFrontLeft; }
    QVector3D m_GetTopBackRight() const { return m_TopBackRight; }
    QVector3D m_GetTopBackLeft() const { return m_TopBackLeft; }
    QVector3D m_GetBottomFrontRight() const { return m_BottomFrontRight; }
    QVector3D m_GetBottomFrontLeft() const { return m_BottomFrontLeft; }
    QVector3D m_GetBottomBackRight() const { return m_BottomBackRight; }
    QVector3D m_GetBottomBackLeft() const { return m_BottomBackLeft; }
    double m_dGetHeight() const { return m_dHeight; }
    double m_dGetWidth() const { return m_dWidth; }
    double m_dGetDepth() const { return m_dDepth; }

private:
    double m_dHeight;
    double m_dWidth;
    double m_dDepth;
    QVector3D m_WorldCenter;
    // relativ zum Center der Welt
    QVector3D m_TopFrontRight;
    QVector3D m_TopFrontLeft;
    QVector3D m_TopBackRight;
    QVector3D m_TopBackLeft;
    QVector3D m_BottomFrontRight;
    QVector3D m_BottomFrontLeft;
    QVector3D m_BottomBackRight;
    QVector3D m_BottomBackLeft;
};

#endif // GEOCUBEBASE_H
