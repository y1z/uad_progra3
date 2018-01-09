#pragma once

#ifndef CVECTOR3_H
#define CVECTOR3_H

// Simple class to practice C++ class creation
// It will be more helpful later when more code is added
class CVector3
{
private:
	float m_X, m_Y, m_Z;

public:
	CVector3();
	CVector3(float _x, float _y, float _z);
	CVector3(const CVector3 &other);
	~CVector3();

	CVector3& operator=(const CVector3 &other);
	CVector3& operator+=(const CVector3 &other);

	void setValues(float _x, float _y, float _z);
	void setValues(float *values);
	float getX() const { return m_X; }
	float getY() const { return m_Y; }
	float getZ() const { return m_Z; }
	void getValues(float *values) const { values[0] = m_X; values[1] = m_Y; values[2] = m_Z; }

	inline static CVector3 ZeroVector()
	{
		return CVector3(0.0f, 0.0f, 0.0f);
	}
};

#endif // !CVECTOR3_H
