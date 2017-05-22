#pragma once
#include "Vector3D.h"
//light class
class Light3D
{
public:
	Light3D(void);
	~Light3D();
	Vector3D position;// position of light object
	//koeficients for colour shading
	unsigned int ambient;
	unsigned int diffuse;
	unsigned int specular;
};