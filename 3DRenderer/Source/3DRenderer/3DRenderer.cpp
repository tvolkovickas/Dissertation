// 3DRenderer.cpp : Defines the entry point for the application.
//Application to load and render a 3D object
//Created by student number: 100188150
//Date 04/01/2013

#include "stdafx.h"
#include "3DRenderer.h"
#include "Vector3D.h"
#include "Vertex.h"
#include "Matrix4.h"
#include "Polygon.h"
#include "Mesh3D.h"
#include<vector>
#include<fstream>
#include<sstream>
#include<algorithm>
#include "Renderer.h"
#include <CommDlg.h>
#include "Light3D.h"



#define MAX_LOADSTRING 100
#define ID_TIMER 999


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
UINT Timer;
float ROTATION=30; //variables for rotation
float ZOOM=10;  // variable to use in projection matrix to make the object bigger or smaller
OPENFILENAME ofn; // used in bitmap saving

unsigned int BRUSHCOL=RGB(255,0,0);// default brush colour


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void ObjectLoader(Mesh3D &mesh,TCHAR objectName[MAX_LOADSTRING] );// function to read and load object
BOOL DibFileSaveDlg(HWND hWnd,PTSTR pstrFileName,PTSTR pstrTitleName);// file save dialog function
void DibFileInitialize(HWND hWnd);// to initialize bitmap for saving
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY3DRENDERER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DRENDERER));

	// Main message loop:
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DRENDERER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY3DRENDERER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

  Timer=SetTimer(hWnd,ID_TIMER,10,NULL);//creates timer for roation

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	static HDC backDC;

	static HBITMAP backbuffer;//bitmap for double buffering

	static RECT clientRect;	

	// rendering objerts
	static Mesh3D myMesh;
	static Renderer renderer;

	//vectors to use in camera matrix
	static Vector3D eye(0.0,0.0,0.0,1);
	static Vector3D at(0.0,0.0,-1.0,1);
	static Vector3D up(0.0,-1.0,0.0,1);


	static Vector3D translation(0,0,-5,1);//translation vector
	//light object
	static Light3D lightObject;
	static Vector3D lightPosition(10,0,-30);

	//boolean values for menu to know what should be on
	static bool lights,wireFrame,rotation;

	//colour selection values
	static CHOOSECOLOR cc;           
	static COLORREF acrCustClr[16]; 
	static DWORD rgbCurrent;   
	static OPENFILENAME ofn; 
	static TCHAR szFile[260];

	static HMENU myMenu=GetMenu(hWnd);

	//bitmap saving values
	static TCHAR szFileName [MAX_PATH],  szTitleName[MAX_PATH];
	static BITMAP bmpScreen;
	static BITMAPFILEHEADER   bmfHeader;    
    static BITMAPINFOHEADER   bi;
	
    
	switch (message)
	{
	case WM_CREATE:			
		ObjectLoader(myMesh,L"teddy.obj");		//to load the default object
		lightObject.position=lightPosition;

		DibFileInitialize(hWnd);

		//colour picker initiliazation
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = hWnd;
		cc.lpCustColors = (LPDWORD) acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter =L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		//checks the default menu values
		CheckMenuItem(myMenu,ID_MODEL_LIGHTNING,MF_CHECKED);
		CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_CHECKED);
		lights=true;
		wireFrame=false;
		rotation=true;
	
		break;
	case WM_SIZE:
		//a bitmap for new client size
		hdc=GetDC(hWnd);
		GetClientRect(hWnd,&clientRect);
		backDC=CreateCompatibleDC(hdc);
		backbuffer=CreateCompatibleBitmap(hdc,clientRect.right,clientRect.bottom);
		SelectObject(backDC,backbuffer);
		ReleaseDC(hWnd,hdc);	
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_FILE_SAVE:
			if(GetMenuState(myMenu,ID_MODEL_ROTATION,MF_BYCOMMAND)==MF_CHECKED)//if object is rotating, stop it
			{		
				CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_UNCHECKED);
			    rotation=false;
			}			
			if(DibFileSaveDlg(hWnd,szFileName,szTitleName))//open file dialog and bitmap structure
			{	
				GetObject(backbuffer,sizeof(BITMAP),&bmpScreen);
				bi.biSize = sizeof(BITMAPINFOHEADER);  
				bi.biWidth = bmpScreen.bmWidth; 
				bi.biHeight = bmpScreen.bmHeight;  
				bi.biPlanes = 1; 
				bi.biBitCount = 32;   
				bi.biCompression = BI_RGB;   
				bi.biSizeImage = 0; 
				bi.biXPelsPerMeter = 0;  
				bi.biYPelsPerMeter = 0;  
				bi.biClrUsed = 0; 
				bi.biClrImportant = 0;
				DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
				HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
				char *lpbitmap = (char *)GlobalLock(hDIB);
				GetDIBits(backDC, backbuffer, 0,(UINT)bmpScreen.bmHeight,lpbitmap,(BITMAPINFO *)&bi, DIB_RGB_COLORS);
				HANDLE hFile = CreateFile(szFileName,GENERIC_WRITE, 0,  NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
    // Add the size of the headers to the size of the bitmap to get the total file size
				DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
    //Offset to where the actual bitmap bits start.
				bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
    
    //Size of the file
				bmfHeader.bfSize = dwSizeofDIB; 
    
    //bfType must always be BM for Bitmaps
				bmfHeader.bfType = 0x4D42; //BM  
				DWORD dwBytesWritten = 0;
				WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    
    //Unlock and Free the DIB from the heap
				GlobalUnlock(hDIB); 
				GlobalFree(hDIB);

    //Close the handle for the file that was created
				CloseHandle(hFile);	
				
			
				CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_CHECKED);//enable rotation after saving
			    rotation=true;
			
			}
			CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_CHECKED);//enable rotation even if the save file was not succesful
			    rotation=true;
			
			break;
		case ID_MODEL_ROTATION:
			if(GetMenuState(myMenu,ID_MODEL_ROTATION,MF_BYCOMMAND)==MF_CHECKED)
			{
		
				CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_UNCHECKED);
			    rotation=false;
			}
			else
			{
			
				CheckMenuItem(myMenu,ID_MODEL_ROTATION,MF_CHECKED);
			    rotation=true;
			}
			
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_MODEL_COLOUR:
			if (ChooseColor(&cc)==TRUE) //returns the colour for brush
			{
				BRUSHCOL=cc.rgbResult; 
				rgbCurrent = cc.rgbResult; 			
			}
			break;
		case ID_MODEL_LOAD:
			if (GetOpenFileName(&ofn)==TRUE) //opens the new object and clears the existing buffers
			{
				myMesh.polygonBuffer.clear();
				myMesh.vertexBuffer.clear();
			
				ObjectLoader(myMesh,ofn.lpstrFile);
			}
			break;
		case ID_MODEL_WIREFRAME://to draw object in wireframe mode
			if(GetMenuState(myMenu,ID_MODEL_WIREFRAME,MF_BYCOMMAND)==MF_CHECKED)
			{
		
				CheckMenuItem(myMenu,ID_MODEL_WIREFRAME,MF_UNCHECKED);
			    wireFrame=false;
			}
			else
			{
			
				CheckMenuItem(myMenu,ID_MODEL_WIREFRAME,MF_CHECKED);
			    wireFrame=true;
			}
			break;
		case ID_MODEL_LIGHTNING://enable lightning
			if(GetMenuState(myMenu,ID_MODEL_LIGHTNING,MF_BYCOMMAND)==MF_CHECKED)
			{
				CheckMenuItem(myMenu,ID_MODEL_LIGHTNING,MF_UNCHECKED);
				lights=false;
			}
			else
			{
				CheckMenuItem(myMenu,ID_MODEL_LIGHTNING,MF_CHECKED);
				lights=true;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;	
	case WM_MOUSEWHEEL:
		{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if(zDelta>0)ZOOM+=1.5;		
		else 
		{
				if(ZOOM>1)
				{
				ZOOM-=1.5;
				}
		}
			
		
		}
		break;
	case WM_TIMER:
		
	   InvalidateRect(hWnd,NULL,true);
		break;

	case WM_PAINT:	

		PatBlt (backDC, 0, 0, clientRect.right, clientRect.bottom, WHITENESS) ;
		renderer.SetModelToWorld(translation,ROTATION);//apply model to world
		if(rotation)//if rotation enabled, update the rotation
		{
		ROTATION+=2;
		if(ROTATION==360) ROTATION=0;
		}
		renderer.SetView(eye,at,up);//apply view
		renderer.SetProjection(ZOOM);//apply projection
		GetClientRect(hWnd,&clientRect);
		renderer.SetViewPort(clientRect);//apply viewport
		renderer.Draw(myMesh,backDC,BRUSHCOL,lightObject,lights,wireFrame);//draw
	
		hdc = BeginPaint(hWnd, &ps);
		
	BitBlt(hdc,0,0,clientRect.right,clientRect.bottom,backDC,0,0,SRCCOPY);// copy the content of bitmap to the screen
		
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		 
//		DeleteObject(brush);
		break;
	case WM_DESTROY:		
		KillTimer(hWnd,ID_TIMER);		
		DeleteDC(backDC);		
		DeleteObject(backbuffer);
	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void ObjectLoader(Mesh3D &mesh,TCHAR objectName[MAX_LOADSTRING])
{
	char op;
	int id1,id2,id3;
	float x,y,z;
	char streamBuff[50];
	Polygon3D polygon;
	Vector3D vectorToBuff;		

	
	//reads the file and extracts the vertices values
	std::ifstream infile(objectName,std::ifstream::in);
	while(!infile.eof())
	{
		infile.getline(streamBuff,50);	
		std::istringstream iss(streamBuff,std::istringstream::in);	
		iss>>op;
		if(op=='v')
		{
			iss>>x>>y>>z;
			vectorToBuff.SetVector(x,y,z);
			mesh.vertexBuffer.push_back(vectorToBuff);
			
		}
		if(op=='f')
		{//construct the polygon from the correspond vertices
			iss>>id1>>id2>>id3;
			polygon.SetPolygon(mesh.vertexBuffer[id1-1],mesh.vertexBuffer[id2-1],mesh.vertexBuffer[id3-1]);
			mesh.polygonBuffer.push_back(polygon);
			
		}
		iss.clear();
	
	}
	infile.close();
	mesh.vertexBuffer.clear();
}
BOOL DibFileSaveDlg(HWND hWnd,PTSTR pstrFileName,PTSTR pstrTitleName)
{
	ofn.hwndOwner=hWnd;
	ofn.lpstrFile=pstrFileName;
	ofn.lpstrFileTitle=pstrTitleName;
	ofn.Flags=OFN_OVERWRITEPROMPT;

	return GetSaveFileName(&ofn);
}

void DibFileInitialize(HWND hWnd)
{
	static TCHAR szFilter[]=TEXT ("Bitmap Files (*.BMP\0*.bmp\0")\
		TEXT ("All Files(*.*)\0*.*\0\0");

	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=hWnd;
	ofn.hInstance=NULL;
	ofn.lpstrFilter=szFilter;
	ofn.lpstrCustomFilter=NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=0;
	ofn.lpstrFile=0;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=MAX_PATH;
	ofn.lpstrInitialDir=NULL;
	ofn.lpstrTitle=NULL;
	ofn.Flags=0;
	ofn.nFileOffset=0;
	ofn.nFileExtension=0;
	ofn.lpstrDefExt=TEXT("bmp");
	ofn.lCustData=0;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
}
