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

#ifndef _DEFINES_H
#define _DEFINES_H

//#include <QString>

// Konstanten
#define DTOR    0.0174532925
#define RTOD    57.2957795
#define TWOPI   6.283185307179586476925287
#define PI      3.141592653589793238462643
#define PID2    1.570796326794896619231322
#define ESC     27
#define EndLine "\r\n"

// Makrofunktionen
#define ABS(x) (x<0?-(x):(x))
#define MIN(x,y) (x<y?x:y)
#define MAX(x,y) (x>y?x:y)
#define SIGN(x) (x<0?(-1):1)
#define MODULUS(p) (sqrt(p.x*p.x+p.y*p.y+p.z*p.z))
#define CROSSPROD(p1,p2,p3) p3.x = p1.y*p2.z - p1.z*p2.y; p3.y = p1.z*p2.x - p1.x*p2.z; p3.z = p1.x*p2.y - p1.y*p2.x
#define NORMALIZE(vector) {double lenght=sqrt(vector.x*vector.x+vector.y*vector.y+vector.z*vector.z); if(lenght==0){vector.x=0;vector.y=0;vector.z=0;}else{vector.x=vector.x/lenght;vector.y=vector.y/lenght;vector.z=vector.z/lenght;}}
#define CLAMP(val,min,max) {if(val>max) val = max; if(val<min) val = min;}

// Default Camera
#define DefaultCamPosX 0.0
#define DefaultCamPosY 0.0
#define DefaultCamPosZ -2.0
#define DefaultCamTarX 0.0
#define DefaultCamTarY 0.0
#define DefaultCamTarZ 0.0
#define DefaultCamUpX 0.0
#define DefaultCamUpY 1.0
#define DefaultCamUpZ 0.0

// Camera-Movment Inkremtentconstants
#define ZoomFactorIncerment 0.002
#define KeyMoveIncrement 0.002
#ifndef _WIN32
    #define unsigned char unsigned char
#endif

// Auflistungen
enum StereoMethod {Anaglyp=0, PolarisationLeft=1, PolarisationRight=2};
enum LogMessageTyp {Debug=0, Warning=1, Error=2, Load=3};
enum LogStackMovment {FuncStayAt=0, FuncUp=1, FuncDown=2};
enum HistogramScalings {Linear=0, Logarithmical=1};

// Strukturen
typedef struct {double Top, Bottom, Left, Right;} Frustum; // Das Frustum für die Camera(quasi sowas wie die Bildfläche)
typedef struct {unsigned char Gray, Data;} TFNode; // Note-Struct for Transferfunction, keeps a r/g/b/a value for a gray value

// Struct für Stereosettings
typedef struct {bool m_bStereoOn; double m_dStereoBasis, m_dFernpunktweite; StereoMethod m_StereoMethod;} StereoSettings;

// strreverse(..) is used in MainWidget and OpenImgStackDialog ... i pasted it direct into this
// classes to avoid the enormouse compile time + alot of compiler warnings ;)
//static QString strreverse(QString in)
//{
//    QString out;
//    int len=in.length();
//    for(int i=len-1, j=0; i>=0; i--, j++)
//    {
//        // exchange elements
//        out[j]=in[i];
//    }
//    return out;
//}

#endif // _DEFINES_H
