#pragma once
//forward declaration
class CAppHexgrid;


#include "stdafx.h"
#include "CGridCell.h"
#include "CAppHexgrid.h"
#include "Include/CGameWindow.h"
#include "Include/Globals.h"
#include "Include/CWideStringHelper.h"

/*!
\class CGrid CGrid.h
\brief this class represent all the hexagon composing 
the grid and is in-charge of initialization and rendering 
of the grid, however this class need to be provided a 
"COpenGLRenderer" and "CGameWindow" to achieve this goal.
*/



class CGrid
{
public:	CGrid(COpenGLRenderer* ptr_renderer, CGameWindow *ptr_GameWindow, 
							uint32_t HexCount, CAppHexgrid* Hexgrid);
				CGrid();
	~CGrid();
protected://variables 

	COpenGLRenderer *mptr_Render = nullptr;
	CGameWindow *mptr_GameWindow = nullptr;
public:
	uint32_t m_CountHexagon = 1;

	bool m_init;//<! checks if we have the color initialized
	
	unsigned int m_ColorProgramID;
	unsigned int m_VertexArrayObjectID;

	uint32_t m_hexGridWidth = 0;
	uint32_t m_hexGridHight = 0;

	int m_hexGridFaces = 6;

	CAppHexgrid * mptr_hexGrid = nullptr;

	CVector3* m_Position =  new CVector3{ 1,1,1 };

	double m_objectRotation;

public:// functions
	bool Init();
	bool Start();
	void Render();
	void CreateGrid();

	/*!
	\brief This function take a pointer and take all 
	the values from a vector a puts them in the pointer 
		\param Vec 
		\param ptr_Out 
	*/
	void GetVertices(std::vector< CGridCell> &Vec,float *&ptr_Out);
};