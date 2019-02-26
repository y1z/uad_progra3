// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
using namespace std;
// -------------------------default includes----------------//
#include "Include/CGameWindow.h"
#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
//--------------------------- My includes ----------------//
#include "CAppHexgrid.h"
#include "CGridCell.h"
int main()
{

	CGridCell test(1);

	test.CreateFlatTopHexagon();
	test.PrintPositions();

	test.MoveDownRight();
	test.PrintPositions();

	test.MoveUpRight();
	test.PrintPositions();

	CApp *app = NULL;    // Pointer to BASE class CApp
	app = new CAppCubeTest();  // Using pointer to base class, create a new object of DERIVED class
	app->run();                        // Run the app
	delete app;                        // Delete pointer
	app = NULL;                        // Set pointer to NULL

	return 0;
}

