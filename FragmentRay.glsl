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

vec3 BlinnPhong(vec3 N, vec3 V, vec3 L)
{
    vec3 Ka = vec3(0.1, 0.1, 0.1);
    vec3 Kd = vec3(0.6, 0.6, 0.6);
    vec3 Ks = vec3(0.2, 0.2, 0.2);
    float n = 100.0;
    vec3 LightColor = vec3(1.0, 1.0, 1.0);
    vec3 LightColorAmbient = vec3(0.3, 0.3, 0.3);

    vec3 H = normalize(L+V);

    vec3 AmbientTeil = Ka * LightColorAmbient;

    float DiffuseLicht = max(dot(L,N), 0.0);
    vec3 DiffuseTeil = Kd * LightColor * DiffuseLicht;

    float SpecularLicht = pow(max(dot(H,N), 0.0), n);
    if(DiffuseLicht <= 0.0) SpecularLicht = 0.0;
    vec3 SpecularTeil = Ks * LightColor * SpecularLicht;

    return AmbientTeil + DiffuseTeil + SpecularTeil;
}

vec3 CookTorrance(vec3 N, vec3 V, vec3 L)
{
    vec3 Kd = vec3(0.6, 0.6, 0.6);
    vec3 Ks = vec3(0.2, 0.2, 0.2);
    float mean = 0.7;
    float scale = 0.2;

    vec3 lightIntensity = vec3(1.0, 1.0, 1.0);

    vec3 H = normalize(L+V);
    float n_h = dot(N,H);
    float n_v = dot(N,V);
    float v_h = dot(V,H);
    float n_l = dot(N,L);

    vec3 diffuse = Kd * max(n_l, 0);
    float fresnel = pow(1.0 + v_h, 4);
    float delta = acos(n_h).x;
    float exponent = -pow((delta/mean), 2);
    float microfacets = scale * exp(exponent);
    float term1 = 2 * n_h * n_v/v_h;
    float term2 = 2 * n_h * n_l/v_h;
    float selfshadow = min(1, min(term1,term2));
    vec3 specular = Ks * fresnel * microfacets * selfshadow / n_v;

    return lightIntensity * (diffuse + specular);
}

void main()
{
    vec4 value;
    vec4 dst = vec4(0, 0, 0, 0);
    float scalar;
    float stepsize = 0.005;
    vec3 volExtentMin = vec3(0,0,0);
    //vec3 volExtentMax = vec3(1,1,1);
    vec3 volExtentMax = vec3(1.0/ufCubeWidth, 1.0/ufCubeDepth, 1.0/ufCubeHeight);

    // this direction must be calculated for the texture coord too! but how? ;D
    // it is the cause of problems for volumes with different Height/Width/Depth
    // and the failure stretch of an cubic volume on the borders of the cube...
    vec3 Direction = vv3Direction;
    vec3 DirectionTex = vv3Direction;
    vec3 DirectionStep = Direction * stepsize;
    vec3 DirectionTexStep = DirectionTex * stepsize;

    vec3 positionvertex = vv3Position;
    vec3 position = vv3Position;
    position.x = (positionvertex.x + 0.5) * (1.0/ufCubeWidth);
    position.y = (positionvertex.y + 0.5) * (1.0/ufCubeHeight);
    position.z = (positionvertex.z + 0.5) * (1.0/ufCubeDepth);

    for(int i=0; i < (1/stepsize); i++)
    {
        scalar = texture3D(VolumeTexture, position).r;
        vec4 src = texture1D(ColorTable, scalar);
        if(src.a > 0)
        {
            vec3 LightDirection  = (vv3Camera+vec3(0.0, 1.0, 0.0));
            //vec3 LightDirection  = normalize(vec3(0.0, 2.0, 0.0) - positionvertex);
            vec3 N = normalize(2.0*src.xyz - vec3(1.0, 1.0, 1.0));
            vec3 L = normalize(LightDirection - positionvertex);
            vec3 V = normalize(vv3Camera - positionvertex);
            vec3 IlluminationTeil = CookTorrance(N, V, L);// BlinnPhong(N,V,L);
            src.x += IlluminationTeil.x;
            src.y += IlluminationTeil.y;
            src.z += IlluminationTeil.z;

            // The new color can just be visible with the free part of the alphafaktor of color b4
            dst = ((1.0-dst.a) * src) + dst;

            if (dst.a >= 0.95)
                break;
        }

        positionvertex = positionvertex + DirectionStep;
        position = position + DirectionTexStep;
        /*vec3 temp1 = sign(position - volExtentMin);
        vec3 temp2 = sign(volExtentMax - position);
        float inside = dot(temp1, temp2);
        if (inside < 3.0)
            break;*/
        if(position.x > 1.0 || position.y > 1.0 || position.z > 1.0 ||
           position.x < 0.0 || position.y < 0.0 || position.z < 0.0) break;
    }

    // Builds a grayscale for anaglyp projections
    if(ubGrayScale == true)
    {
        scalar = (dst.r + dst.g + dst.b)/3.0;
        dst.r = scalar;
        dst.g = scalar;
        dst.b = scalar;
    }
    gl_FragColor = dst;
};
