#include "stdafx.h"
#include "Vertex.h"
//constructors

Vertex::Vertex(void)
{
	position.x=0;
	position.y=0;
	position.z=0;
	position.w=1;
//	colour=RGB(255,255,255); default colour for per vertex lightning
}

Vertex::Vertex(Vector3D v)
{
	position.x=v.x;
	position.y=v.y;
	position.z=v.z;
	position.w=v.w;
//	colour=RGB(255,255,255); default colour for per vertex lightning
}
Vertex::~Vertex()
{
}




