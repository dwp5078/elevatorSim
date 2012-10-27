
#ifndef __MVECTORS_HPP__
#define __MVECTORS_HPP__

#include <iostream>
#include <math.h>
#include <limits>
#include <assert.h>

using namespace std;

const float MY_PI = 4.0f * atan(1.0f);

namespace elevatorSim   {
   class Vec3f;
   class Vec4f;
   class Mat44;

   class Vec3f
   {
   private:

   public:
      float x, y, z;

      Vec3f()  {x = y = z = 0;}

      Vec3f(const Vec3f &v)   {
         x = v.x;
         y = v.y;
         z = v.z;
      }

      Vec3f(float dx, float dy, float dz) {
         x = dx;
         y = dy;
         z = dz;
      }

      ~Vec3f() {}

      void Set(float dx, float dy, float dz) {
         x = dx;
         y = dy;
         z = dz;
      }

      void Set(const Vec3f &v)   {
         x = v.x;
         y = v.y;
         z = v.z;
      }

      void Scale(float d)  {
         x *= d;
         y *= d;
         z *= d;
      }

      Vec3f& operator = (const Vec3f &v)  {
         x = v.x;
         y = v.y;
         z = v.z;
         return *this;
      }

      bool operator == (const Vec3f &v) const   {
         return ( fabs(x - v.x) < numeric_limits<float>::epsilon( ) &&
            fabs(y - v.y) < numeric_limits<float>::epsilon( ) &&
            fabs(z - v.z) < numeric_limits<float>::epsilon( ));
      }

      bool operator != (const Vec3f &v) const {
         return ( fabs(x - v.x) > numeric_limits<float>::epsilon( ) ||
            fabs(y - v.y) > numeric_limits<float>::epsilon( ) ||
            fabs(z - v.z) > numeric_limits<float>::epsilon( ));
      }

      Vec3f& operator += (const Vec3f &v) {
         x += v.x;
         y += v.y;
         z += v.z;
         return *this;
      }

      Vec3f& operator -= (const Vec3f &v) {
         x -= v.x;
         y -= v.y;
         z -= v.z;
         return *this;
      }

      Vec3f& operator /= (const Vec3f &v) {
         x /= v.x;
         y /= v.y;
         z /= v.z;
         return *this;
      }

      Vec3f& operator /= (float f) {
         x /= f;
         y /= f;
         z /= f;
         return *this;
      }

      Vec3f& operator *= (const Vec3f &v) {
         x *= v.x;
         y *= v.y;
         z *= v.z;
         return *this;
      }

      Vec3f& operator *= (float f) {
         x *= f;
         y *= f;
         z *= f;
         return *this;
      }

      /*friend void CopyScale(Vec3f &a, const Vec3f &b, float c ) {
      a.x = b.x * c;
      a.y = b.y * c;
      a.z = b.z * c;
      }*/

      friend Vec3f operator+(const Vec3f &v1, const Vec3f &v2) {
         return Vec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
      }

      friend Vec3f operator - (const Vec3f &v1, const Vec3f &v2) {
         return Vec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
      }

      friend Vec3f operator * (const Vec3f &v1, float f) {
         return Vec3f(v1.x * f, v1.y * f, v1.z * f);
      }

      friend Vec3f operator / (const Vec3f &v1, float f) {
         return Vec3f(v1.x / f, v1.y / f, v1.z / f);
      }

      float Length() {
         return (float)sqrt(x * x + y * y + z * z);
      }

      void Normalize()  {
         float l = Length();
         if(l > 0)   {
            x /= l;
            y /= l;
            z /= l;
         }
      }

      float Dot(const Vec3f &v) const {
         return x * v.x + y * v.y + z * v.z;
      }

      static void Cross3(Vec3f &c, const Vec3f &v1, const Vec3f &v2)   {
         float dx = v1.y * v2.z - v1.z * v2.y;
         float dy = v1.z * v2.x - v1.x * v2.z;
         float dz = v1.x * v2.y - v1.y * v2.x;
         c.x = dx; c.y = dy; c.z = dz;
      }


   }; //end of Vec3f

   class Vec4f
   {
   private:

   public:
      float x, y, z, w;

      Vec4f()  {x = y = z = 0;   w = 1;}

      Vec4f(const Vec3f &v)   {
         x = v.x;      y = v.y;      z = v.z;      w = 1;
      }

      Vec4f(const Vec4f &v)   {
         x = v.x;      y = v.y;      z = v.z;      w = v.w;
      }

      Vec4f(float dx, float dy, float dz) {
         x = dx;      y = dy;      z = dz;      w = 1;
      }

      Vec4f(float dx, float dy, float dz, float dw) {
         x = dx;      y = dy;      z = dz;      w = dw;
      }

      ~Vec4f() {}

      inline void Set(float dx, float dy, float dz, float dw) {
         x = dx;      y = dy;      z = dz;      w = dw;
      }

      inline void Set(const Vec3f &v)   {
         x = v.x;      y = v.y;      z = v.z;      w = 1;
      }

      inline void Set(const Vec4f &v)   {
         x = v.x;      y = v.y;      z = v.z;      w = v.w;
      }

      inline void Scale(float d)  {
         x *= d;     y *= d;      z *= d;      w *= d;
      }

      inline Vec4f& operator = (const Vec4f &v)  {
         x = v.x;
         y = v.y;
         z = v.z;
         w = v.w;
         return *this;
      }

      inline bool operator == (const Vec4f &v) const   {
         return ( fabs(x - v.x) < numeric_limits<float>::epsilon( ) &&
            fabs(y - v.y) < numeric_limits<float>::epsilon( ) &&
            fabs(z - v.z) < numeric_limits<float>::epsilon( ) &&
            fabs(w - v.w) < numeric_limits<float>::epsilon( ));
      }

      inline bool operator != (const Vec4f &v) const {
         return ( fabs(x - v.x) > numeric_limits<float>::epsilon( ) ||
            fabs(y - v.y) > numeric_limits<float>::epsilon( ) ||
            fabs(z - v.z) > numeric_limits<float>::epsilon( ) ||
            fabs(w - v.w) > numeric_limits<float>::epsilon( ));
      }

      inline Vec4f& operator += (const Vec4f &v) {
         x += v.x;
         y += v.y;
         z += v.z;
         w += v.w;
         return *this;
      }

      inline Vec4f& operator -= (const Vec4f &v) {
         x -= v.x;
         y -= v.y;
         z -= v.z;
         w -= v.w;
         return *this;
      }

      inline Vec4f& operator /= (const Vec4f &v) {
         x /= v.x;
         y /= v.y;
         z /= v.z;
         w /= v.w;
         return *this;
      }

      inline Vec4f& operator /= (float f) {
         x /= f;
         y /= f;
         z /= f;
         w /= f;
         return *this;
      }

      inline Vec4f& operator *= (const Vec4f &v) {
         x *= v.x;
         y *= v.y;
         z *= v.z;
         w *= v.w;
         return *this;
      }

      inline Vec4f& operator *= (float f) {
         x *= f;
         y *= f;
         z *= f;
         w *= f;
         return *this;
      }

      inline friend Vec4f operator+(const Vec4f &v1, const Vec4f &v2) {
         return Vec4f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
      }

      inline friend Vec4f operator - (const Vec4f &v1, const Vec4f &v2) {
         return Vec4f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
      }

      inline friend Vec4f operator * (const Vec4f &v1, float f) {
         return Vec4f(v1.x * f, v1.y * f, v1.z * f, v1.w * f);
      }

      inline friend Vec4f operator / (const Vec4f &v1, float f) {
         return Vec4f(v1.x / f, v1.y / f, v1.z / f, v1.w / f);
      }

      inline float Length() {
         return (float)sqrt(x * x + y * y + z * z + w * w);
      }

      inline void Normalize()  {
         float l = Length();
         if(l > 0)   {
            x /= l;
            y /= l;
            z /= l;
            w /= l;
         }
      }

      inline float Dot(const Vec4f &v) const {
         return x * v.x + y * v.y + z * v.z + w * v.w;
      }
   }; //end of Vec4f


   class Mat44
   {
   public:
      float m[4][4];

   public:
      Mat44()  {
         for(int i=0; i<4; i++)  {
            for (int j=0; j<4; j++) {
               //identity matrix;
               if(i == j)  m[i][j] = 1.0f;
               else        m[i][j] = 0.0f;
            }
         }
      }

      Mat44(const Mat44 &dm)   {
         memcpy(m, &dm, sizeof(float)*16);
      }

      ~Mat44();

      inline friend Mat44 operator + (const Mat44& m1, const Mat44& m2){
         Mat44 C;
         for(int i=0; i<4; i++)  {
            for(int j=0; j<4; j++)  {
               C.m[i][j] = m1.m[i][j] + m2.m[i][j];
            }
         }

         return C;
      }

      inline friend Mat44 operator - (const Mat44& m1, const Mat44& m2){
         Mat44 C;
         for(int i=0; i<4; i++)  {
            for(int j=0; j<4; j++)  {
               C.m[i][j] = m1.m[i][j] - m2.m[i][j];
            }
         }

         return C;
      }

      inline friend Mat44 operator * (const Mat44& A, const Mat44& B){
         Mat44 C;

         C.m[0][0] = A.m[0][0] * B.m[0][0] + A.m[0][1] * B.m[1][0] + A.m[0][2] * B.m[2][0] + A.m[0][3] * B.m[3][0];
         C.m[0][1] = A.m[0][0] * B.m[0][1] + A.m[0][1] * B.m[1][1] + A.m[0][2] * B.m[2][1] + A.m[0][3] * B.m[3][1];
         C.m[0][2] = A.m[0][0] * B.m[0][2] + A.m[0][1] * B.m[1][2] + A.m[0][2] * B.m[2][2] + A.m[0][3] * B.m[3][2];
         C.m[0][3] = A.m[0][0] * B.m[0][3] + A.m[0][1] * B.m[1][3] + A.m[0][2] * B.m[2][3] + A.m[0][3] * B.m[3][3];
         C.m[1][0] = A.m[1][0] * B.m[0][0] + A.m[1][1] * B.m[1][0] + A.m[1][2] * B.m[2][0] + A.m[1][3] * B.m[3][0];
         C.m[1][1] = A.m[1][0] * B.m[0][1] + A.m[1][1] * B.m[1][1] + A.m[1][2] * B.m[2][1] + A.m[1][3] * B.m[3][1];
         C.m[1][2] = A.m[1][0] * B.m[0][2] + A.m[1][1] * B.m[1][2] + A.m[1][2] * B.m[2][2] + A.m[1][3] * B.m[3][2];
         C.m[1][3] = A.m[1][0] * B.m[0][3] + A.m[1][1] * B.m[1][3] + A.m[1][2] * B.m[2][3] + A.m[1][3] * B.m[3][3];
         C.m[2][0] = A.m[2][0] * B.m[0][0] + A.m[2][1] * B.m[1][0] + A.m[2][2] * B.m[2][0] + A.m[2][3] * B.m[3][0];
         C.m[2][1] = A.m[2][0] * B.m[0][1] + A.m[2][1] * B.m[1][1] + A.m[2][2] * B.m[2][1] + A.m[2][3] * B.m[3][1];
         C.m[2][2] = A.m[2][0] * B.m[0][2] + A.m[2][1] * B.m[1][2] + A.m[2][2] * B.m[2][2] + A.m[2][3] * B.m[3][2];
         C.m[2][3] = A.m[2][0] * B.m[0][3] + A.m[2][1] * B.m[1][3] + A.m[2][2] * B.m[2][3] + A.m[2][3] * B.m[3][3];
         C.m[3][0] = A.m[3][0] * B.m[0][0] + A.m[3][1] * B.m[1][0] + A.m[3][2] * B.m[2][0] + A.m[3][3] * B.m[3][0];
         C.m[3][1] = A.m[3][0] * B.m[0][1] + A.m[3][1] * B.m[1][1] + A.m[3][2] * B.m[2][1] + A.m[3][3] * B.m[3][1];
         C.m[3][2] = A.m[3][0] * B.m[0][2] + A.m[3][1] * B.m[1][2] + A.m[3][2] * B.m[2][2] + A.m[3][3] * B.m[3][2];
         C.m[3][3] = A.m[3][0] * B.m[0][3] + A.m[3][1] * B.m[1][3] + A.m[3][2] * B.m[2][3] + A.m[3][3] * B.m[3][3];

         return C;
      }

      inline friend Vec4f operator * (const Mat44& A, const Vec4f& v){
         Vec4f T;
         T.x = A.m[0][0] * v.x + A.m[0][1]*v.y + A.m[0][2]*v.z + A.m[0][3]*v.w;
         T.y = A.m[1][0] * v.x + A.m[1][1]*v.y + A.m[1][2]*v.z + A.m[1][3]*v.w;
         T.z = A.m[2][0] * v.x + A.m[2][1]*v.y + A.m[2][2]*v.z + A.m[2][3]*v.w;
         T.w = A.m[3][0] * v.x + A.m[3][1]*v.y + A.m[3][2]*v.z + A.m[3][3]*v.w;

         return T;
      }

      inline friend Vec3f operator * (const Mat44& A, const Vec3f& v){
         Vec3f v1;

         v1.x = A.m[0][0] * v.x + A.m[0][1]*v.y + A.m[0][2]*v.z;
         v1.y = A.m[1][0] * v.x + A.m[1][1]*v.y + A.m[1][2]*v.z;
         v1.z = A.m[2][0] * v.x + A.m[2][1]*v.y + A.m[2][2]*v.z;

         return v1;
      }

      inline void RotationX(Mat44 &A, float fangle)
      {
         /*
         |1  0    0    0|
         |0  cos  sin  0|
         |0 -sin  cos  0|
         |0  0    0    1|*/

         A.m[1][1] = cos(fangle);
         A.m[1][2] = sin(fangle);
         A.m[2][1] = -sin(fangle);
         A.m[2][2] = cos(fangle);
      }

      inline void RotationY(Mat44 &A, float fangle)
      {
         /*
         |cos  0 -sin  0|
         |0    1  0    0|
         |sin  0  cos  0|
         |0    0  0    1|*/

         A.m[0][0] = cos(fangle);
         A.m[0][2] = -sin(fangle);
         A.m[2][0] = sin(fangle);
         A.m[2][2] = cos(fangle);
      }

      inline void RotationZ(Mat44 &A, float fangle)
      {
         /*
         |cos  sin 0   0|
         |-sin cos 0   0|
         |0    0   1   0|
         |0    0   0   1|*/

         A.m[0][0] = cos(fangle);
         A.m[0][1] = sin(fangle);
         A.m[1][0] = -sin(fangle);
         A.m[1][1] = cos(fangle);
      }

      inline void RotationVec(Mat44 &A, const Vec3f &v, float fangle)
      {
         A.Identity();
         //v.Normalize();

         A.m[0][0] = 1.0f + (1.0f-cos(fangle))*(v.x*v.x-1);
         A.m[0][1] = -v.z*sin(fangle)+(1.0f-cos(fangle))*v.x*v.y;
         A.m[0][2] = v.y*sin(fangle)+(1.0f-cos(fangle))*v.x*v.z;
         A.m[1][0] = v.z*sin(fangle)+(1.0f-cos(fangle))*v.x*v.y;
         A.m[1][1] = 1.0f+(1.0f-cos(fangle))*(v.y*v.y-1.0f);
         A.m[1][2] = -v.x*sin(fangle)+(1.0f-cos(fangle))*v.y*v.z;
         A.m[2][0] = -v.y*sin(fangle)+(1.0f-cos(fangle))*v.x*v.z;
         A.m[2][1] = v.x*sin(fangle)+(1.0f-cos(fangle))*v.y*v.z;
         A.m[2][2] = 1.0f+(1.0f-cos(fangle))*(v.z*v.z-1.0f);
      }

      inline void Identity()   {
         for(int i=0; i<4; i++)  {
            for(int j=0; j<4; j++)  {
               if(i == j)     m[i][j] = 1.0f;
               else           m[i][j] = 0.0f;
            }
         }
      }

   };    //end of Mat44
}


#endif //__MVECTORS_HPP__