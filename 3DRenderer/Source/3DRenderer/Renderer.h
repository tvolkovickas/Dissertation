#pragma once
#include "Matrix4.h"
#include "Mesh3D.h"
#include "Polygon.h"
#include "Light3D.h"
#include "Vector3D.h"
//rendering cladss
class Renderer
{
public:
	Renderer(void);
	~Renderer();
	//setters
	void SetModelToWorld(Vector3D translation,float rotation);
	void SetView(Vector3D eye,Vector3D at,Vector3D up);
	void SetProjection(float scale);
	void SetViewPort(RECT &client);
	//main drawing function
	void Draw(Mesh3D mesh,HDC backDC,unsigned int brushCol,Light3D light,bool lights,bool wire);
	
	

private://private functions
	Mesh3D &ModelToWorld(Mesh3D &mesh);//translate vertices to world coordinates
	Mesh3D &CalculateNormals(Mesh3D &mesh);//calculate normals
	Mesh3D &DepthSort(Mesh3D &mesh);//to do depth sorting
	bool isBackFace(Polygon3D p,Vector3D v);//check if the polygon is back facing
	unsigned int CheckRGB(unsigned int finalCol,unsigned int spectacular);//checks if the rgb values does go beyond the 255
	unsigned int Irradiance(Polygon3D p,Vector3D n,Vector3D v,Light3D li,int power);//enables lightning 
	
	inline int Round(float f)//funtion to convert the float to integer in more efficient way
	{
		int i;
		__asm
		{
			fld f
			fistp i
		}
		return i;
	}
	//objects
	Matrix4 modelToWorld;
	Matrix4 viewMatrix;
	Matrix4 projection;
	Matrix4 viewport;
	Vector3D viewVector;
	
};