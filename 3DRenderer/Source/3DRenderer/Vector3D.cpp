#include "stdafx.h"
#include "Vector3D.h"
#include <math.h>
//overloading operators
Vector3D Vector3D::operator+(const Vector3D &rhs) const
{
	return Vector3D(x+rhs.x, y+rhs.y,z+rhs.z,w+rhs.w);
}

Vector3D Vector3D::operator-(const Vector3D &rhs) const
{
	return Vector3D(x-rhs.x, y-rhs.y,z-rhs.z,w-rhs.w);
}

//dot produnct of two vectors
float Vector3D::dot(const Vector3D &rhs) const
{
	return (x*rhs.x+y*rhs.y+z*rhs.z+w*rhs.w);
}


//to normalize the vector
Vector3D Vector3D::normal() 
{
	float fLength =sqrt( x * x + y * y + z * z );//vector lenght
	//unit vector
	x=x/fLength;
	y=y/fLength;
	z=z/fLength;
	return Vector3D(x,y,z,w);
}
//used in calculating normals
Vector3D Vector3D::operator/(const float a) const
{
	return Vector3D(x/a,y/a,z/a,w/a);
}
//cross product
void Vector3D::cross( Vector3D &v,Vector3D &v1)
{
	
                  x= v.y * v1.z - v.z * v1.y;
                 y=  v.z * v1.x - v.x * v1.z;
                 z=  v.x * v1.y - v.y * v1.x;
				   w=1.0f;

}
//setter
void Vector3D::SetVector(float x1,float y1,float z1)
{
	x=x1;
	y=y1;
	z=z1;
	w=1;
}