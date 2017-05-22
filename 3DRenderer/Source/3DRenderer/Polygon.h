#pragma once
#include "stdafx.h"
#include "Vertex.h"
#include"Vector3D.h"
//polygon class


class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(Vertex v1,Vertex v2,Vertex v3);//constructor that takes 3 vertices	
	void SetPolygon(Vertex v1,Vertex v2,Vertex v3);//setter	
	void SetNormal(Vector3D v1,Vector3D v2,Vector3D v3);//to set normal for polygon	
	float GetMinZvalue();// function to get the min value from all the vertices in a polygon

	//variables
	Vertex vertexBuffer[3];
	Vector3D mNormal;
	//ambient, diffuse and spectacular colours for polygon
	unsigned int ka;
	unsigned int kd;
	unsigned int ks;
	//final colour of polygon
	unsigned int colour;
};