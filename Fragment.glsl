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
uniform vec3 PosLicht;
uniform vec3 PosCamera;
uniform bool bGrayScale;

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
    vec4 vecpos = texture3D(VolumeTexture, gl_TexCoord[0].stp);
    //float gray = (vecpos.r + vecpos.g + vecpos.b)/3.0;
    vec4 vecres = texture1D(ColorTable, vecpos.r);

    if(vecres.a > 0)
    {
        vec3 one = vec3(1.0, 1.0, 1.0);
        vec3 N = normalize(2.0*vecres.xyz - one);
        vec3 L = normalize(PosLicht - gl_TexCoord[0].stp);
        vec3 V = normalize(PosCamera - gl_TexCoord[0].stp);
        //vec3 IlluminationTeil = BlinnPhong(N,V,L);
        vec3 IlluminationTeil = CookTorrance(N,V,L);
        vecres.x += IlluminationTeil.x;
        vecres.y += IlluminationTeil.y;
        vecres.z += IlluminationTeil.z;
        if(bGrayScale == true)
        {
            float stereogray = (vecres.r + vecres.g + vecres.b)/3.0;
            vecres.r = stereogray;
            vecres.g = stereogray;
            vecres.b = stereogray;
        }
    }
    gl_FragColor = vecres;
};
