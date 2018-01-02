#pragma once

#ifndef CAPPPARCIAL2_H
#define CAPPPARCIAL2_H

#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"

// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
class CAppParcial2 : public CApp
{
private:
	// Pointer to an object of type C3DModel
	C3DModel *m_p3DModel;

	//
	int m_animationFrameCounter;

protected:
	// Method to initialize the menu
	bool initializeMenu();

public:
	// Constructors and destructor
	CAppParcial2();
	CAppParcial2(int window_width, int window_height);
	~CAppParcial2();

	// Inherited methods from CApp
	// ---------------------------

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Methods specific to CAppParcial2 only
	// -------------------------------------

	// This derived class only uses F2/F3
	void onF2(int mods);
	void onF3(int mods);
	
private:

	// Load/unload 3D model
	bool load3DModel(const char * const filename);
	void unloadCurrent3DModel();

	// Move camera away/closer
	void moveCamera(float direction);
};

#endif // !CAPPPARCIAL2_H