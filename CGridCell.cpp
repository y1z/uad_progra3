#include "CGridCell.h"

//! default value of m_Size is 1 
CGridCell::CGridCell()
{
	for (char i = 0; i < 6; ++i)
	{
		m_Positions[i].setValues(m_Size, m_Size, 0);
	}
}

//! Set size of all sides of the hexagon
CGridCell::CGridCell(float Size)
	:m_Size(Size)
{
	// for initialization of the values 
	for (char i = 0; i < 6; ++i)
	{
		m_Positions[i].setValues(0, 0, 0);
	}
}

// ! set Size and Position of hexagon 
CGridCell::CGridCell(float Size, float XPosition, float YPosition)
	:m_Size(Size)
{
	// for initialization of the values 
	for (char i = 0; i < 6; ++i)
	{
		m_Positions[i].setValues(0, 0, 0);
	}

	SetCenterPosition(XPosition, YPosition);
}

CGridCell::~CGridCell()
{
}

void CGridCell::CreateFlatTopHexagon()
{
	static constexpr float AnglePostion = 60.0f;//!<use to determine where to
																					//!<locate individual Position for Hexagon
	for (char i = 0; i < 6; ++i)
	{
		float AngleDeg = AnglePostion * i;
		float AngleRad = CONVERT_TO_RADIANS(AngleDeg);
		// assign the position of the points
		m_Positions[i].X = m_CenterPosition.X + m_Size * (cosf(AngleRad));
		// this is because of sin(pi) is inaccurate
		if (i != 3)
		{
			m_Positions[i].Y = m_CenterPosition.Y + m_Size * (sinf(AngleRad));
		}
		else// sin(pi)= 0 or the center of hexagon
		{
			m_Positions[i].Y = m_CenterPosition.Y;
		}
	}

	GetDimensionOfHexagon();
}

void CGridCell::SetCenterPosition(float XPosition, float YPosition)
{
	m_CenterPosition.X = XPosition;
	m_CenterPosition.Y = YPosition;
}

void CGridCell::SetZ(float ZPosition)
{
	m_CenterPosition.Z = ZPosition;
	for (auto& Points : m_Positions)
	{
		Points.Z = ZPosition;
	}
}

float CGridCell::GetHexagonHight() const{
	return m_HexHight;
}

float CGridCell::GetHexagonWidth() const {
	return m_HexWidth;
}

void CGridCell::PrintPositions()
{
	printf("\n-------------------------\n");
	for (int i = 0; i < 6; ++i)
	{
		printf("Position %d <%f , %f> \n", i, m_Positions[i].getX(), m_Positions[i].getY());
	}
	printf("\n-------------------------\n");
}

void CGridCell::GetDimensionOfHexagon()
{
	// gets hight of the hexagon 
	m_HexHight = sqrt(3) * m_Size;
	// gets width of the hexagon 
	m_HexWidth = 2 * m_Size;
}

void CGridCell::operator=(const CGridCell &other)
{
	for (int i = 0; i < 6; ++i)
	{
		this->m_Positions[i] = other.m_Positions[i];
	}

	this->m_CenterPosition = other.m_CenterPosition;
	this->m_HexHight = other.GetHexagonHight();
	this->m_HexWidth = other.GetHexagonWidth();
}

//! To position the hexagon 
void CGridCell::MoveUpRight()
{
	float HorizantalDistace = m_HexWidth * 0.75f;
	float VerticalDistance = m_HexHight * 0.50f;

	m_CenterPosition.Y += VerticalDistance;
	m_CenterPosition.X += HorizantalDistace;

	CreateFlatTopHexagon();
}

void CGridCell::MoveDownRight()
{
	float HorizantalDistace = m_HexWidth * 0.75f;
	float VerticalDistance = m_HexHight * 0.50f;

	m_CenterPosition.Y -= VerticalDistance;
	m_CenterPosition.X += HorizantalDistace;

	CreateFlatTopHexagon();
}

void CGridCell::MoveUp()
{
	float VerticalDistance = m_HexHight;

	m_CenterPosition.Y += VerticalDistance;

	CreateFlatTopHexagon();
}

void CGridCell::TrasnferVertices(float *& ptr_Out, int LoopCount)
{
	// total induces times the current LoopCount 
	int CurrentIndex = 21 * LoopCount;

	// always get the induces of the center first
	ptr_Out[CurrentIndex] = m_CenterPosition.getX();
	CurrentIndex++;
	ptr_Out[CurrentIndex] = m_CenterPosition.getY();
	CurrentIndex++;
	ptr_Out[CurrentIndex] = m_CenterPosition.getZ();

	// the rest of the values  
	for (auto Point : m_Positions)
	{
		CurrentIndex++;
		ptr_Out[CurrentIndex] = Point.getX();
		CurrentIndex++;
		ptr_Out[CurrentIndex] = Point.getY();
		CurrentIndex++;
		ptr_Out[CurrentIndex] = Point.getZ();
	}

}
