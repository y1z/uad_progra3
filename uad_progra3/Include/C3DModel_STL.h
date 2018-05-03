#pragma once
#include "C3DModel.h"
#include "CVector3.h"

#define Header_Size 80

class C3DModel_STL : public C3DModel
{
private:
	FILE * _objFileName;

	CVector3 *m_vertices;                               // Dynamically-allocated array of vertices
	CVector3 *m_normals;                               // Dynamically-allocated array of normals
	CVector3 *m_UVCoords;                             // Dynamically-allocated array of UV coords

	//char headerArr[Header_Size];
	short int headerArr[Header_Size];

public:
	C3DModel_STL();
	~C3DModel_STL();

	bool readFile(const char * const filename); //*** should be private

protected:
	void reset();
	bool loadFromFile(const char * const filename);
};
