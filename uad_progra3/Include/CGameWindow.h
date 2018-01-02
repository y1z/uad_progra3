#pragma once

#ifndef CGAMEWINDOW_H
#define CGAMEWINDOW_H

// include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "COpenGLRenderer.h"

#include <string>
using namespace std;

// Class CGameWindow
class CGameWindow
{
public:
	static const int DEFAULT_WINDOW_WIDTH  = 800;
	static const int DEFAULT_WINDOW_HEIGHT = 600;

private:
	COpenGLRenderer * const m_ReferenceRenderer; // Const pointer to OpenGL renderer. To avoid calling OpenGL methods directly
	bool         m_InitializedGLFW;        // Is GLFW framework initialized
	int          m_Width;                  // Window width
	int          m_Height;                 // Window height
	GLFWwindow * m_Window;                 // Pointer to GLFW window
	string       m_WindowTitle;            // Title 

	                                       // Generic variables for F1..F12 keys, CApp derived class decides what to do for F2..F11
	static bool requestF1;                 // F1 always toggles the menu on/off
	static bool requestF2;                 // 
	static bool requestF3;                 // 
	static bool requestF4;                 //
	static bool requestF5;                 //
	static bool requestF6;                 //
	static bool requestF7;                 //
	static bool requestF8;                 //
	static bool requestF9;                 //
	static bool requestF10;                //
	static bool requestF11;                //
	static bool requestF12;                // F12 always exits the application

	                                       // Specific keys for the actions of the menu
	static bool requestExecuteAction;      // Enter
	static bool requestSelectNextMenuItem; // Key arrow down
	static bool requestSelectPrevMenuItem; // Key arrow up

	static int  keyMods;                   // Key modifiers, they match the GLFW key modifiers (alt, ctrl, shift)

	static int  newWidth;
	static int  newHeight;

	// Initialize GLFW library
	void initializeGLFW();

	// Callbacks in C++ must be static
	// This callback will be called by GLFW when the user resizes the window, so we can set the correct viewport
	static void resizeCallback(GLFWwindow * window, int width, int height);
	// This callback will be called by GLFW when a physical key is pressed in the keyboard
	static void keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

	// Process user input (keyboard, mouse)
	void processInput(void *appPointer);

public:
	// Constructors and Destructor
	CGameWindow(COpenGLRenderer * renderer);
	CGameWindow(COpenGLRenderer * renderer, int width, int height);
	~CGameWindow();

	// Create a window
	bool create(const char *windowTitle);

	// Main loop
	void mainLoop(void *appPointer);

	// Meant to be called from any CApp derived class 
	void requestWindowClose();

	// Is GLFW framework initialized?
	inline bool isLibraryInitialized() const { return m_InitializedGLFW; }

	// Getters for window dimentions
	inline int getWidth() const  { return m_Width; }
	inline int getHeight() const { return m_Height; }
};

#endif // !CGAMEWINDOW_H