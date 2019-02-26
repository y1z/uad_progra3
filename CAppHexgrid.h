#pragma once
#include "stdafx.h"
#include "Include/CApp.h"
#include "Include/Globals.h"

#include "CGrid.h"

class CAppHexgrid : public CApp
{
public:
	CAppHexgrid();
	CAppHexgrid(uint16_t Width, uint16_t Hight);
	~CAppHexgrid();
public:// variables 
	uint32_t m_HexGridHight = 3;
	uint32_t m_HexGridWidth = 3;

	CVector3 m_objectPosition;

	CGrid *ptr_Grid = nullptr;
protected :// necessary virtual functions 
	void initialize() override;
	void update(double delta_time) override;
	void run() override;
	void render() override;
	bool initializeMenu() override;

public:

};

