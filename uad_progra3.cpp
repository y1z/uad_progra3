// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>

using namespace std;


#include "Include/MathHelper.h"
// -------------------------default includes----------------//
#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
//--------------------------- My includes ----------------//
#include "Include/CVector3.h"
#include "SDataContainter.h"
#include "CAppHexgrid.h"
#include "CGridCell.h"
#include "CParser3d.h"
#include "COctNode.h"
#include "COctTree.h"
#include <Windows.h>


int main()
{

	HANDLE Threads[2];
	DWORD ThreadIDs[2];

	FBXContainer DataTrasfer;
	CParser3d parse;
	COctTree OctreeStatic;

	parse.ParseFBXFile("MODELS\\FBX\\Pyro_Head_Ascii.fbx");
	parse.GetModelData(DataTrasfer);

	parse.GetModelData(DataTrasfer);

	OctreeStatic.Model = &DataTrasfer;

	Threads[0] = CreateThread(NULL, 0,
														reinterpret_cast<LPTHREAD_START_ROUTINE> (&COctTree::ThreadFunction),
														reinterpret_cast<LPVOID>(&OctreeStatic), 0,
														&ThreadIDs[0]);

	WaitForSingleObject(Threads[0], INFINITE);


	//int x = 0;

	CApp *app = NULL;    // Pointer to BASE class CApp
	app = new CAppHexgrid(1920, 1080, 3, 3);  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

