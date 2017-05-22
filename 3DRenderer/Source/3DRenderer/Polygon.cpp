#include "stdafx.h"
#include "Polygon.h"
//constructors
Polygon3D::Polygon3D()
{
	vertexBuffer[0].position.x=0;
	vertexBuffer[0].position.y=0;
	vertexBuffer[0].position.z=0;

	vertexBuffer[1].position.x=0;
	vertexBuffer[1].position.y=0;
	vertexBuffer[1].position.z=0;

	vertexBuffer[2].position.x=0;
	vertexBuffer[2].position.y=0;
	vertexBuffer[2].position.z=0;

}

Polygon3D::Polygon3D(Vertex v1,Vertex v2,Vertex v3)
{
	vertexBuffer[0].position.x=v1.position.x;
	vertexBuffer[0].position.y=v1.position.y;
	vertexBuffer[0].position.z=v1.position.z;

	vertexBuffer[1].position.x=v2.position.x;
	vertexBuffer[1].position.y=v2.position.y;
	vertexBuffer[1].position.z=v2.position.z;

	vertexBuffer[2].position.x=v3.position.x;
	vertexBuffer[2].position.y=v3.position.y;
	vertexBuffer[2].position.z=v3.position.z;
}




//setter function
void Polygon3D::SetPolygon(Vertex v1,Vertex v2,Vertex v3)
{
	vertexBuffer[0].position.x=v1.position.x;
	vertexBuffer[0].position.y=v1.position.y;
	vertexBuffer[0].position.z=v1.position.z;
	vertexBuffer[0].position.w=1;

	vertexBuffer[1].position.x=v2.position.x;
	vertexBuffer[1].position.y=v2.position.y;
	vertexBuffer[1].position.z=v2.position.z;
	vertexBuffer[1].position.w=1;

	vertexBuffer[2].position.x=v3.position.x;
	vertexBuffer[2].position.y=v3.position.y;	
	vertexBuffer[2].position.z=v3.position.z;
	vertexBuffer[2].position.w=1;
}
//to set normal
void Polygon3D::SetNormal(Vector3D v1,Vector3D v2,Vector3D v3)
{
	Vector3D vec1(v2.x-v1.x,v2.y-v1.y,v2.z-v1.z);
	Vector3D vec2(v3.x-v1.x,v3.y-v1.y,v3.z-v1.z);
	mNormal.cross(vec1,vec2);
	mNormal.normal();
		
}
//function to get the minimum value from 3 vertices that makes the polygon. To be used in depth sorting algorithm
float Polygon3D::GetMinZvalue()
{
	if(vertexBuffer[0].position.z<vertexBuffer[1].position.z)
	{
		return vertexBuffer[0].position.z;
	}
	else if (vertexBuffer[1].position.z<vertexBuffer[2].position.z)
	{
		return vertexBuffer[1].position.z;
	}
	else return vertexBuffer[2].position.z;
}

