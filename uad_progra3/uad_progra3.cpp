// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;

#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"

int main()
{
	//float FullCircleRadions = 6.283185f;

	//CVector3 Test(2.f, 2.f, 2.f);
	//for(int i = 0; i < 80;++i)
	//{
	//	CVector3 Result = MathHelper::RotateXY(Test, FullCircleRadions);
	//}

	CApp *app = NULL;                  // Pointer to BASE class CApp
	app = new CAppGeometricFigures(800, 600);  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

