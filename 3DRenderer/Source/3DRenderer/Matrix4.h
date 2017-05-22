#pragma once
#include "Vector3D.h"
#include "Vertex.h"
//matrix class
class Matrix4
{
public:
Matrix4(void);
Matrix4(float m00,float m01,float m02,float m03,
		float m10,float m11,float m12,float m13,
		float m20,float m21,float m22,float m23,
		float m30,float m31,float m32,float m33);//constructor that takes 16 values to create matrix
~Matrix4();
//getter and setter
float GetM(int,int)const;
void SetM(int,int,float);
//operators overloading
Matrix4 operator * (const Matrix4 &m2) const;
 Vector3D operator *(const Vector3D &v) const;
Vertex operator*(const Vertex &v) const;

Matrix4   &identityM();//to set the matrix to identity matrix
Matrix4   &translate(const Vector3D& v);//set matrix to translation matrix,uses vector values
Matrix4   &translate(float x, float y, float z);//set matrix to translation matrix,uses float values
//creates to matrices for view matrix
Matrix4   &InvOrientation(const Matrix4 &m1);
Matrix4   &InvTranslation(const Matrix4 &m1);
//set scale matrix
Matrix4   &scale(const Vector3D &v);
//set angle for rotation matrix
Matrix4   &rotateY(float angle);

private:
	float m[4][4];	
};