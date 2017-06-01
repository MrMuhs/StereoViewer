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

#include "GeoCubeBase.h"

GeoCubeBase::GeoCubeBase(QVector3D center, double height, double width, double depth)
{
    m_WorldCenter = center;
    m_dHeight = height;
    m_dWidth = width;
    m_dDepth = depth;
    m_TopFrontLeft  = QVector3D(center.x()-width/2.0, center.y()+height/2.0, center.z()-depth/2.0);
    m_TopFrontRight = QVector3D(center.x()+width/2.0, center.y()+height/2.0, center.z()-depth/2.0);
    m_TopBackLeft = QVector3D(center.x()-width/2.0, center.y()+height/2.0, center.z()+depth/2.0);
    m_TopBackRight = QVector3D(center.x()+width/2.0, center.y()+height/2.0, center.z()+depth/2.0);
    m_BottomFrontLeft  = QVector3D(center.x()-width/2.0, center.y()-height/2.0, center.z()-depth/2.0);
    m_BottomFrontRight = QVector3D(center.x()+width/2.0, center.y()-height/2.0, center.z()-depth/2.0);
    m_BottomBackLeft = QVector3D(center.x()-width/2.0, center.y()-height/2.0, center.z()+depth/2.0);
    m_BottomBackRight = QVector3D(center.x()+width/2.0, center.y()-height/2.0, center.z()+depth/2.0);
}
