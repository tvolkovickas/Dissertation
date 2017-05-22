#include "stdafx.h"
#include "Matrix4.h"
#include<math.h>
#define RAD 3.14/180
//constructors
Matrix4::Matrix4(void)
{
	m[0][0]=0;
	m[0][1]=0;
	m[0][2]=0;
	m[0][3]=0;
	m[1][0]=0;
	m[1][1]=0;
	m[1][2]=0;
	m[1][3]=0;
	m[2][0]=0;
	m[2][1]=0;
	m[2][2]=0;
	m[2][3]=0;
	m[3][0]=0;
	m[3][1]=0;
	m[3][2]=0;
	m[3][3]=0;
}
Matrix4::Matrix4(float m00,float m01,float m02,float m03,
		float m10,float m11,float m12,float m13,
		float m20,float m21,float m22,float m23,
		float m30,float m31,float m32,float m33)
{
	m[0][0]=m00;
	m[0][1]=m01;
	m[0][2]=m02;
	m[0][3]=m03;
	m[1][0]=m10;
	m[1][1]=m11;
	m[1][2]=m12;
	m[1][3]=m13;
	m[2][0]=m20;
	m[2][1]=m21;
	m[2][2]=m22;
	m[2][3]=m23;
	m[3][0]=m30;
	m[3][1]=m31;
	m[3][2]=m32;
	m[3][3]=m33;
}

Matrix4::~Matrix4(void)
{
}
//getter
float Matrix4::GetM(int a,int b)const
{
	return m[a][b];
}
//setter
void Matrix4::SetM(int a,int b,float value)
{
	m[a][b]=value;
}

Matrix4 Matrix4::operator*(const Matrix4 &m2) const
{
	Matrix4 r;
	r.m[0][0]=m[0][0]*m2.m[0][0]+m[0][1] * m2.m[1][0]+m[0][2] * m2.m[2][0]+m[0][3] * m2.m[3][0];
	r.m[0][1]=m[0][0]*m2.m[0][1]+m[0][1] * m2.m[1][1]+m[0][2] * m2.m[2][1]+m[0][3] * m2.m[3][1];
	r.m[0][2]=m[0][0]*m2.m[0][2]+m[0][1] * m2.m[1][2]+m[0][2] * m2.m[2][2]+m[0][3] * m2.m[3][2];
	r.m[0][3]=m[0][0]*m2.m[0][3]+m[0][1] * m2.m[1][3]+m[0][2] * m2.m[2][3]+m[0][3] * m2.m[3][3];

	r.m[1][0]=m[1][0]*m2.m[0][0]+m[1][1] * m2.m[1][0]+m[1][2] * m2.m[2][0]+m[1][3] * m2.m[3][0];
	r.m[1][1]=m[1][0]*m2.m[0][1]+m[1][1] * m2.m[1][1]+m[1][2] * m2.m[2][1]+m[1][3] * m2.m[3][1];
	r.m[1][2]=m[1][0]*m2.m[0][2]+m[1][1] * m2.m[1][2]+m[1][2] * m2.m[2][2]+m[1][3] * m2.m[3][2];
	r.m[1][3]=m[1][0]*m2.m[0][3]+m[1][1] * m2.m[1][3]+m[1][2] * m2.m[2][3]+m[1][3] * m2.m[3][3];

	r.m[2][0]=m[2][0]*m2.m[0][0]+m[2][1] * m2.m[1][0]+m[2][2] * m2.m[2][0]+m[2][3] * m2.m[3][0];
	r.m[2][1]=m[2][0]*m2.m[0][1]+m[2][1] * m2.m[1][1]+m[2][2] * m2.m[2][1]+m[2][3] * m2.m[3][1];
	r.m[2][2]=m[2][0]*m2.m[0][2]+m[2][1] * m2.m[1][2]+m[2][2] * m2.m[2][2]+m[2][3] * m2.m[3][2];
	r.m[2][3]=m[2][0]*m2.m[0][3]+m[2][1] * m2.m[1][3]+m[2][2] * m2.m[2][3]+m[2][3] * m2.m[3][3];

	r.m[3][0]=m[3][0]*m2.m[0][0]+m[3][1] * m2.m[1][0]+m[3][2] * m2.m[2][0]+m[3][3] * m2.m[3][0];
	r.m[3][1]=m[3][0]*m2.m[0][1]+m[3][1] * m2.m[1][1]+m[3][2] * m2.m[2][1]+m[3][3] * m2.m[3][1];
	r.m[3][2]=m[3][0]*m2.m[0][2]+m[3][1] * m2.m[1][2]+m[3][2] * m2.m[2][2]+m[3][3] * m2.m[3][2];
	r.m[3][3]=m[3][0]*m2.m[0][3]+m[3][1] * m2.m[1][3]+m[3][2] * m2.m[2][3]+m[3][3] * m2.m[3][3];

	return r;
}
//takes argument of vecot and passes to translate function
Matrix4& Matrix4::translate(const Vector3D& v)
{
    return translate(v.x, v.y, v.z);
}
//set matrix to identity matrix
Matrix4 &Matrix4::identityM()
{
	m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
    m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
    m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    return *this;
}
//set the translation
Matrix4& Matrix4::translate(float x, float y, float z)
{
    m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = x;
    m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = y;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = z;
    m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    return *this;
}
//set sclaling matrix
Matrix4& Matrix4::scale(const Vector3D& v)
{
	m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;

	return *this;
}
//set rotation matrix around Y axis
Matrix4 &Matrix4::rotateY(float angle)
{
	float a=cosf(angle*RAD);
	float b=sinf(angle*RAD);

	m[0][0]=a;
	m[0][2]=b;
	m[2][0]=-b;
	m[2][2]=a;

	return *this;
}

 Vector3D Matrix4::operator*(const Vector3D &v) const
{
	return Vector3D(
                   m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                   m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                   m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                   m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
                   );

}

Vertex Matrix4::operator*(const Vertex &v) const
{
	return Vector3D(
                   m[0][0] * v.position.x + m[0][1] * v.position.y + m[0][2] * v.position.z + m[0][3] * v.position.w, 
                   m[1][0] * v.position.x + m[1][1] * v.position.y + m[1][2] * v.position.z + m[1][3] * v.position.w,
                   m[2][0] * v.position.x + m[2][1] * v.position.y + m[2][2] * v.position.z + m[2][3] * v.position.w,
                   m[3][0] * v.position.x + m[3][1] * v.position.y + m[3][2] * v.position.z + m[3][3] * v.position.w
                   );

}
//creates matrices for view matrix
Matrix4& Matrix4::InvOrientation(const Matrix4 &m)
{
	Matrix4 orientation(
		m.GetM(0,0),  m.GetM(1,0),  m.GetM(2,0),  0,
        m.GetM(0,1),  m.GetM(1,1),  m.GetM(2,1),  0,
        m.GetM(0,2),  m.GetM(1,2),  m.GetM(2,2),   0,
        0.0,  0.0,  0.0, 1.0);
	return orientation;
}

Matrix4& Matrix4::InvTranslation(const Matrix4 &m)
{
	Matrix4 translation(
		1.0,  0.0,  0.0,  -m.GetM(0,3),
        0.0,  1.0,  0.0,  -m.GetM(1,3),
        0.0,  0.0, 1.0,   -m.GetM(2,3),
        0.0,  0.0,  0.0,           1.0);
	return translation;

}