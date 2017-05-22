#pragma once
#include "stdafx.h"
#include "Vector3D.h"
//class to hold the information about the vertex object
class Vertex
{
	public:
		Vertex(void);	
		Vertex(Vector3D v);
		~Vertex();

		//variables
		Vector3D position;
		unsigned int colour; // vertex colour to be used for per vertex shading


};