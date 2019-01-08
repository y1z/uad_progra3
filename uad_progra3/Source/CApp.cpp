#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "../Include/CApp.h"
#include "../Include/LoadTGA.h"

/* */
CApp::CApp():
	CApp(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CApp::CApp(int window_width, int window_height)
{
	cout << "Constructor: CApp(int window_width, int window_height)" << endl;
	
	// Create OpenGLRenderer Object
	m_OpenGLRenderer = new COpenGLRenderer();

	// Create CGameWindow object
	m_Window = new CGameWindow(m_OpenGLRenderer, window_width, window_height);

	// Create CGameMenu Object
	// Menu will be initialized later (menu items added)
	m_Menu = new CGameMenu();
}

/* */
CApp::~CApp()
{
	cout << "Destructor: ~CApp()" << endl;

	// Free CGameWindow object resources
	if (m_Window != NULL)
	{
		cout << "Delete m_Window" << endl;
		delete m_Window;
	}

	m_Window = NULL;

	// Free CGameMenu object resources
	if (m_Menu != NULL)
	{
		// Cleanup call deleteVertexArrayObject
		m_Menu->cleanupGraphicsObjects(m_OpenGLRenderer);

		cout << "Delete m_Menu" << endl;
		delete m_Menu;
	}

	m_Menu = NULL;

	// Free COpenGLRenderer object resources
	if (m_OpenGLRenderer != NULL)
	{
		cout << "Delete m_OpenGLRenderer" << endl;
		delete m_OpenGLRenderer;
	}

	m_OpenGLRenderer = NULL;
}

/* */
bool CApp::isWindowInitialized() const
{
	if (m_Window != NULL)
	{
		return m_Window->isLibraryInitialized();
	}

	return false;
}

/* */
bool CApp::canRun() const
{
	return isWindowInitialized();
}

/* */
bool CApp::isMenuActive() const
{
	if (m_Menu != NULL)
	{
		return m_Menu->isActive();
	}

	return false;
}

/* */
void CApp::setMenuActive(bool active)
{
	if (m_Menu != NULL)
	{
		m_Menu->setActive(active);

		if (active)
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.88f, 0.88f, 0.88f);
			}

			cout << "Menu is active" << endl;
			cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
		}
		else
		{
			if (m_OpenGLRenderer != NULL)
			{
				m_OpenGLRenderer->setClearScreenColor(0.15f, 0.75f, 0.75f);
			}

			cout << "Menu is NOT active" << endl;
		}
	}
}

/* */
void CApp::executeMenuAction()
{
	if (m_Menu != NULL)
	{
		cout << "Execute Menu action" << endl;
	}
}

/* */
void CApp::selectNextMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(true);
		cout << "Select next menu option" << endl;
		cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}

/* */
void CApp::selectPrevMenuItem()
{
	if (m_Menu != NULL)
	{
		m_Menu->selectMenuItem(false);
		cout << "Select prev menu option" << endl;
		cout << "Selected option is #" << m_Menu->getSelectedMenuItemNumber() << endl;
	}
}

/* Read texture file and generate an OpenGL texture object */
bool CApp::loadTexture(const char *filename, unsigned int *newTextureID)
{
	TGAFILE tgaFile;
	tgaFile.imageData = nullptr;

	if (filename == nullptr || newTextureID == nullptr || m_OpenGLRenderer == nullptr)
	{
		return false;
	}

	*newTextureID = 0;

	if (LoadTGAFile(filename, &tgaFile))
	{
		if (tgaFile.imageData == nullptr ||
			tgaFile.imageHeight < 0 ||
			tgaFile.imageWidth < 0)
		{
			if (tgaFile.imageData != nullptr)
			{
				delete[] tgaFile.imageData;
			}

			return false;
		}

		// Create a texture object for the menu, and copy the texture data to graphics memory
		if (!m_OpenGLRenderer->createTextureObject(
			newTextureID,
			tgaFile.imageData,
			tgaFile.imageWidth,
			tgaFile.imageHeight
		))
		{
			return false;
		}

		// Texture data is stored in graphics memory now, we don't need this copy anymore
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}
	}
	else
	{
		// Free texture data
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}

		return false;
	}

	return true;
}