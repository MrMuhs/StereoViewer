#ifndef __gluLookAtReplacement_h__
#define __gluLookAtReplacement_h__

#include <GL/gl.h>

static void identity(GLfloat m[16])
{
   m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
   m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
   m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
   m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

static void normalize(float v[3])
{
   float r = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
   if(r == 0.0) return;

   v[0] /= r;
   v[1] /= r;
   v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
   result[0] = v1[1]*v2[2] - v1[2]*v2[1];
   result[1] = v1[2]*v2[0] - v1[0]*v2[2];
   result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

// this should work like gluLookAt from the lib, i guess
void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
             GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
             GLdouble upz)
{
   float forward[3], side[3], up[3];
   GLfloat m[4][4];

   forward[0] = centerx - eyex;
   forward[1] = centery - eyey;
   forward[2] = centerz - eyez;

   up[0] = upx;
   up[1] = upy;
   up[2] = upz;

   normalize(forward);

   /* Side = forward x up */
   cross(forward, up, side);
   normalize(side);

   /* Recompute up as: up = side x forward */
   cross(side, forward, up);

   identity(&m[0][0]);
   m[0][0] = side[0];
   m[1][0] = side[1];
   m[2][0] = side[2];

   m[0][1] = up[0];
   m[1][1] = up[1];
   m[2][1] = up[2];

   m[0][2] = -forward[0];
   m[1][2] = -forward[1];
   m[2][2] = -forward[2];

   glMultMatrixf(&m[0][0]);
   glTranslated(-eyex, -eyey, -eyez);
}

#endif