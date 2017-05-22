#pragma once
#include <vector>
#include "Vertex.h"
#include "Polygon.h"
//mesh class
class Mesh3D
{
public:
	Mesh3D(void);
	~Mesh3D();
	//vertex and polygon buffers
	std::vector<Vertex>vertexBuffer;
	std::vector<Polygon3D>polygonBuffer;
};