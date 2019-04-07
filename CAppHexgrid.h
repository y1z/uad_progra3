#pragma once
#include "stdafx.h"
#include "Include/CApp.h"
#include "Include/Globals.h"

#include "CGrid.h"

/*! \class CAppHexgrid  CAppHexgrid.h "CAppHexgrid.h"
	\brief This class is used to initialize hexagrid, but the
	render of the hexagrid is taken care of by CGrid
	\author Yhaliff Said Barraza Zubia
	\date 05/March/2019
	\version 1.00
*/

class CGrid;

class CAppHexgrid : public CApp
{
public:
	CAppHexgrid();
	CAppHexgrid(uint16_t ScreenWidth, uint16_t ScreenHight);

	CAppHexgrid(uint16_t ScreenWidth,
							uint16_t ScreenHight,
							uint32_t HexGridWidth,
							uint32_t hexGridhight);
	~CAppHexgrid();
public:// variables 
	uint32_t m_HexGridHight = 1;
	uint32_t m_HexGridWidth = 1;

	CVector3 m_objectPosition;

	CGrid *mptr_Grid = nullptr;

protected:// necessary virtual functions 
	void initialize() override;
	void update(double delta_time) override;
	void run() override;
	void render() override;
	bool initializeMenu() override;
};

