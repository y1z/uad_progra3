/*!
\brief this class is where the geometry of the hexagon 
is stored	and calculated
*/
#pragma once
#include "stdafx.h"
#include "Include/CVector3.h"
#include "Include/Globals.h"
class CGridCell
{
public:// constructors and destructor
	CGridCell();
	CGridCell(float Size);
	CGridCell(float Size, float XPosition, float YPosition);
	~CGridCell();
public:// functions 
	/*!
	\fn CreateFlatTopHexagon this method will calculate the position of the 6 points 
	necessary for a hexagon
*/
	void CreateFlatTopHexagon();

	void SetCenterPosition(float XPosition, float YPosition);

	float GetHexagonWidth() const ;
	float GetHexagonHight() const ;


	void PrintPositions();	// <!Just for debugging

	void MoveUpRight();
	void MoveDownRight();
	void MoveUp();
	
	void TransferValue(float *& ptr_Out, int);
	//--------------------
private:
	void GetDimensionOfHexagon();

public: // Operators 
	void operator= (const CGridCell &other);


protected:// Variables 
	CVector3 m_CenterPosition = { 0.f,0.f,0.f };
	CVector3 m_Positions[6];

	float m_Size = 1.0f;
	float m_HexWidth = 0.f;
	float m_HexHight = 0.f;
};