#pragma once
//vector class
class Vector3D
{
	public:
		Vector3D(): x(0),y(0),z(0),w(0){};
		Vector3D(float x, float y,float z,float w=1.0f): x(x),y(y),z(z),w(w) {};//constructor that takes 3 or 4 values with 4th being default 1
		
		float dot(const Vector3D &vec) const;//dot product
		void SetVector(float x,float y,float z);//setter
		Vector3D normal() ;//to normalise the current vector
		void cross( Vector3D &v1,Vector3D &v2) ;//corss product
		//overloaded operators
		Vector3D operator/(const float a) const;
		Vector3D operator+(const Vector3D &rhs) const;
		Vector3D operator-(const Vector3D &rhs) const;

		
	//variables
		float x;
		float y;
		float z;
		float w;
};