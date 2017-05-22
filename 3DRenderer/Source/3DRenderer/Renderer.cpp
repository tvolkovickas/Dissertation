#include "stdafx.h"
#include "Renderer.h"
#include <algorithm>

Renderer::Renderer(void)
{

}

Renderer::~Renderer()
{

}
//setting model to world matrix
void Renderer::SetModelToWorld(Vector3D translation,float rotation)
{
	modelToWorld.identityM();

	Matrix4 translationM;

	translationM.identityM();	
	translationM.translate(translation);

	Matrix4 rotationM;
	rotationM.identityM();	
	rotationM.rotateY(rotation);	

	modelToWorld=translationM*rotationM;
}
//setting view matrix
void Renderer::SetView(Vector3D eye,Vector3D at,Vector3D up)
{
	Vector3D view;
	view=at-eye;
	view.normal();
	viewVector=view;

	Vector3D right;
	right.cross(view,up);
	right.normal();

	Vector3D upVec;
	upVec.cross(right,view);
	upVec.normal();	

	Matrix4 orientation(right.x,right.y,right.z,0,
						upVec.x,upVec.y,upVec.z,0,
						view.x,view.y,view.z,0,
						0,0,0,1);
	
	Matrix4 cameraTranslation(0,0,0,eye.x,
		                      0,0,0,eye.y,
							  0,0,0,eye.z,
							  0,0,0,    1);

	orientation=orientation.InvOrientation(orientation);//to inverse the orientation matrix
	cameraTranslation=cameraTranslation.InvTranslation(cameraTranslation);//to inverse the camera transslation matrix

	
	viewMatrix=cameraTranslation*orientation;
}
//projection matrix
void Renderer::SetProjection(float scale)
{
	projection.identityM();
	projection.SetM(0,0,scale);
	projection.SetM(1,1,scale);
	projection.SetM(2,2,0.0f);
}
//viewport matrix that translates object into the middle of the screen
void Renderer::SetViewPort(RECT &client)
{
	viewport.identityM();
	viewport.SetM(0,3,client.right/2);
	viewport.SetM(1,3,client.bottom/2);
	
}
//main drawing function
void Renderer::Draw(Mesh3D mesh,HDC backDC,unsigned int brushCol,Light3D light,bool lights,bool wire)
{
	int polygonBuffsize;
	POINT p1[3];
	//transform vertices
	mesh=ModelToWorld(mesh);
	mesh=CalculateNormals(mesh);
	mesh=DepthSort(mesh);

	unsigned int col;

	SelectObject(backDC, GetStockObject(DC_PEN));
	SelectObject(backDC, GetStockObject(DC_BRUSH));

	
	polygonBuffsize=mesh.polygonBuffer.size();

	for (int i=0;i<polygonBuffsize;i++)
	{
		if(isBackFace(mesh.polygonBuffer[i],viewVector))
		{
			//setting the polygon colour
			mesh.polygonBuffer[i].ka=brushCol;
			mesh.polygonBuffer[i].kd=brushCol;
			mesh.polygonBuffer[i].ks=brushCol;
			if(lights)
			{
			col=Irradiance(mesh.polygonBuffer[i],mesh.polygonBuffer[i].mNormal,viewVector,light,10);//return value is the new colour
			//for the polygon
			}
			else col=brushCol;

			//multyplying each vertex by view and projection matrices and assigning x and y values to the point structure
			mesh.polygonBuffer[i].vertexBuffer[0]=viewMatrix*mesh.polygonBuffer[i].vertexBuffer[0];
		    mesh.polygonBuffer[i].vertexBuffer[0]=viewport*projection*mesh.polygonBuffer[i].vertexBuffer[0];
			 p1[0].x=Round(mesh.polygonBuffer[i].vertexBuffer[0].position.x);	
			p1[0].y=Round(mesh.polygonBuffer[i].vertexBuffer[0].position.y);
			
			mesh.polygonBuffer[i].vertexBuffer[1]=viewMatrix*mesh.polygonBuffer[i].vertexBuffer[1];
			mesh.polygonBuffer[i].vertexBuffer[1]=viewport*projection*mesh.polygonBuffer[i].vertexBuffer[1];
			p1[1].x=Round(mesh.polygonBuffer[i].vertexBuffer[1].position.x);
		   p1[1].y=Round(mesh.polygonBuffer[i].vertexBuffer[1].position.y);

			mesh.polygonBuffer[i].vertexBuffer[2]=viewMatrix*mesh.polygonBuffer[i].vertexBuffer[2];	
			mesh.polygonBuffer[i].vertexBuffer[2]=viewport*projection*mesh.polygonBuffer[i].vertexBuffer[2];
			p1[2].x=Round(mesh.polygonBuffer[i].vertexBuffer[2].position.x);
		    p1[2].y=Round(mesh.polygonBuffer[i].vertexBuffer[2].position.y);		
		
		 SetDCBrushColor(backDC, col);
		 if(wire)SetDCPenColor(backDC, RGB(0,0,0));
		 else SetDCPenColor(backDC, col);

		 //GDI draw polygon function
		 Polygon(backDC,p1,3);
		 
		}
	}
	
}
//applying model to word matrix to every vertex
Mesh3D &Renderer::ModelToWorld(Mesh3D &mesh)
{
	int polygonBuffsize;
		
		
		polygonBuffsize=mesh.polygonBuffer.size();
	
	for (int i=0;i<polygonBuffsize;i++)
	{		
		
		mesh.polygonBuffer[i].vertexBuffer[0]=modelToWorld*mesh.polygonBuffer[i].vertexBuffer[0];
		mesh.polygonBuffer[i].vertexBuffer[1]=modelToWorld*mesh.polygonBuffer[i].vertexBuffer[1];
		mesh.polygonBuffer[i].vertexBuffer[2]=modelToWorld*mesh.polygonBuffer[i].vertexBuffer[2];					
				
	
	}
	return mesh;
}
//calculates normals
Mesh3D &Renderer::CalculateNormals(Mesh3D &mesh)
{
	int polygonBuffsize;
		
		
	polygonBuffsize=mesh.polygonBuffer.size();

	for(int i=0;i<polygonBuffsize;i++)
	{
		mesh.polygonBuffer[i].SetNormal(mesh.polygonBuffer[i].vertexBuffer[2].position,
				                       mesh.polygonBuffer[i].vertexBuffer[1].position,
				                     mesh.polygonBuffer[i].vertexBuffer[0].position);

	}

	return mesh;
}
//sorts polygons by the returne z min value
Mesh3D &Renderer::DepthSort(Mesh3D &mesh)
{
	std::sort(mesh.polygonBuffer.begin(),mesh.polygonBuffer.end(),[](Polygon3D  a,Polygon3D b
		){return a.GetMinZvalue()<b.GetMinZvalue();});

	return mesh;
}
//checks if the polygon is back facing
bool Renderer::isBackFace(Polygon3D p,Vector3D v)
{

	float d=p.mNormal.dot(v);

	if(d<0)
	{
		return false;

	}
	else return true;
}
//function to calculate the final colour for the polygon
unsigned int Renderer::Irradiance(Polygon3D p,Vector3D n,Vector3D v,Light3D li,int power)
{
	unsigned int a; //ambient
	unsigned int d;// diffuse
	unsigned int s;//spectacular
	
	
	float k;

	Vector3D h;

	//ambient component
	a=RGB((GetRValue(p.ka)*0.2),(GetGValue(p.ka)*0.2),(GetBValue(p.ka)*0.2));

	//diffuse component
	li.position=li.position-n;
	li.position.normal();
	k=li.position.dot(n);
	if(k>1.0) k=1.0;
	else if (k<0) k=0;

	d=RGB(GetRValue(p.kd)*0.5*k,GetGValue(p.kd)*0.5*k,GetBValue(p.kd)*0.5*k);
	

	//spectacular component
	float fLength =sqrt( (v.x+li.position.x) * (v.x+li.position.x) + (v.y+li.position.y) * (v.y+li.position.y)
		+ (v.z+li.position.z) * (v.z+li.position.z) );
	h=(v+li.position)/fLength ;
	h.normal();
	s=RGB(GetRValue(p.ks)*0.8*pow(n.dot(h),power),GetGValue(p.ks)*0.8*pow(n.dot(h),power),GetBValue(p.ks)*0.8*pow(n.dot(h),power));

	p.colour=a+d;
	
	p.colour=CheckRGB(p.colour,s);// checks if the RGB value does not go beyond the 255
	
	return p.colour;
}
//checks the rgb value
unsigned int Renderer::CheckRGB(unsigned int finalCol,unsigned int spectacular)
{
	unsigned int r,g,b;
	if(GetRValue(spectacular)+GetRValue(finalCol)>255)
	{
		
		r=255;
	}
	else
	{
		r=GetRValue(spectacular)+GetRValue(finalCol);
	}

	if(GetGValue(spectacular)+GetGValue(finalCol)>255)
	{
		
		g=255;
	}
	else
	{
		g=GetGValue(spectacular)+GetGValue(finalCol);
	}
	

	if(GetBValue(spectacular)+GetBValue(finalCol)>255)
	{
		
		b=255;
	}
	else
	{
		b=GetBValue(spectacular)+GetBValue(finalCol);
	}
	

	return finalCol=RGB(r,g,b);

}
