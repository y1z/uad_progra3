#pragma once
#include <cinttypes>

/*! 
\brief this is a struct which catain all the data necesary for 
a model 
*/

struct FBXContainer 
{
	uint32_t m_ShaderID = 0;
	uint32_t m_ObjectID = 0;
	
	int32_t m_numFaces = 0;//!< how many face a model has 
	uint32_t m_numNormals = 0;//!< how many normal it has 
	uint32_t m_numVertices = 0;//!< how many vertices it has 
	uint32_t m_numUVs = 0;//!<how many UVs it has 
	uint32_t m_numUVIndex = 0;

	float *mptr_Vertices = nullptr;
	float *mptr_Normals = nullptr;
	float *mptr_UVCoords = nullptr;

	unsigned short * mptr_VertriceIndice = nullptr;
	unsigned short * mptr_UVIndice = nullptr;
	unsigned short * mptr_NormalIndice = nullptr;

};