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

uniform sampler3D VolumeTexture;
uniform sampler1D ColorTable;
uniform vec3 uv3PosLicht;
uniform float ufPosCameraX;
uniform float ufPosCameraY;
uniform float ufPosCameraZ;
uniform float ufCubeWidth;
uniform float ufCubeHeight;
uniform float ufCubeDepth;
uniform bool ubGrayScale;

varying vec3 vv3Direction;
varying vec3 vv3Camera;
varying vec3 vv3Position;

void main() 
{ 
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;

    vv3Camera = vec3(ufPosCameraX, ufPosCameraY, ufPosCameraZ);
    vv3Direction = gl_Vertex - vv3Camera;
    vv3Direction = normalize(vv3Direction);
    vv3Position = gl_Vertex;
};
