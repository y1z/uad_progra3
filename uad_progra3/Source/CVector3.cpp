#include "../stdafx.h"

#include "../Include/CVector3.h"

/*
*/
CVector3::CVector3() 
	: m_X(0.0f), 
	m_Y(0.0f), 
	m_Z(0.0f) 
{
}

/*
*/
CVector3::CVector3(float _x, float _y, float _z)
	: m_X(_x), m_Y(_y), m_Z(_z)
{
}

/*
*/
CVector3::CVector3(const CVector3 &other)
{
	m_X = other.getX();
	m_Y = other.getY();
	m_Z = other.getZ();
}

/*
*/
CVector3& CVector3::operator=(const CVector3 &other)
{
	m_X = other.getX();
	m_Y = other.getY();
	m_Z = other.getZ();

	return *this;
}

/*
*/
CVector3& CVector3::operator+=(const CVector3 &other)
{
	m_X += other.getX();
	m_Y += other.getY();
	m_Z += other.getZ();

	return *this;
}

/*
*/
CVector3::~CVector3()
{
}

/*
*/
void CVector3::setValues(float _x, float _y, float _z)
{
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
}

/*
*/
void CVector3::setValues(float *values)
{
	m_X = *(values + 0);
	m_Y = *(values + 1);
	m_Z = *(values + 2);
}