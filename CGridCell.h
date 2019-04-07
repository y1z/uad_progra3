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

	void SetZ(float ZPosition);

	float GetHexagonWidth() const ;
	float GetHexagonHight() const ;


	void PrintPositions();	// <!Just for debugging

	void MoveUpRight();
	void MoveDownRight();
	void MoveUp();
	
	/*! function made to be in a for Loop
		\fn TrasnferVertices
		\param [out] ptr_Out the pointer that will be altered, should be a nullptr
		\param [in] LoopCount to know the iteration of the for loop 
	*/
	void TrasnferVertices(float *& ptr_Out, int LoopCount);
	//--------------------
private:
	void GetDimensionOfHexagon();

public: // Operators 
	void operator= (const CGridCell &other);

	CVector3 m_Positions[6];
protected:// Variables 
	CVector3 m_CenterPosition = { 0.f,0.f,0.f };


	float m_DistanceFromCamera = -1.0f;
	float m_Size = 1.0f;
	float m_HexWidth = 0.f;
	float m_HexHight = 0.f;
};