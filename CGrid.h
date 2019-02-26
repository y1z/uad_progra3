#pragma once
#include "stdafx.h"
#include "CGridCell.h"
#include "Include/CGameWindow.h"
#include "Include/Globals.h"
#include "Include/CWideStringHelper.h"

/*!
\class CGrid CGrid.h
\brief this class represent all the hexagon composing 
the grid and is in-charge of initialization and rendering 
of the grid, however this class need to be provided a 
"COpenGLRenderer" and "CGameWindow" achieve this goal,
*/

class CAppHexgrid;

class CGrid
{
public:	CGrid(COpenGLRenderer* ptr_renderer, CGameWindow *ptr_GameWindow, uint32_t HexCount, CAppHexgrid& Hexgrid);
	~CGrid();
public://variables 

	COpenGLRenderer *m_Render = nullptr;
	CGameWindow *m_GameWindow = nullptr;
	uint32_t m_CountHexagon = 1;

	bool m_init;
	
	unsigned int m_ColorProgramID;
	unsigned int m_VertexArrayObjectID;

	uint32_t m_GridWidth = 0;
	uint32_t m_GridHight = 0;

	int m_hexGridFaces = 6;

	CAppHexgrid * m_hexGrid = nullptr;

public:// functions
	bool Init();
	bool Start();
	void Render();
	void CreatGrid();
	void GetVertices(std::vector< CGridCell> &Vec,float *&ptr_Out);
};