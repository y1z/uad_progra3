#pragma once

#ifndef C3DMODEL_3DS_H
#define C3DMODEL_3DS_H

#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include "C3DModel.h"
#include "CVector3.h"

class C3DModel_3DS : public C3DModel
{
private:
	// Methods to read a model in the .obj file format
	//bool readFile(const char * const filename);       // Read object from file
	void computeFaceNormals();

	class C3DS_Parser
	{
	public:
		C3DS_Parser(string filename);
		~C3DS_Parser();

		bool Success = false;

		vector<unsigned short> indices;
		vector<float> verticesRaw;
		vector<float> UVcoordsRaw;
		vector<CVector3> vertices;
		vector<CVector3> UVcoords;
		

	private:
		//>------ Id Chunk

#define MAIN3DS       0x4D4D

		//>------ Main Chunks

#define EDIT3DS       0x3D3D  // this is the start of the editor config
#define KEYF3DS       0xB000  // this is the start of the keyframer config

		//>------ sub defines of EDIT3DS

#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100
#define EDIT_OBJECT   0x4000

#define EDIT_UNKNW01  0x1100
#define EDIT_UNKNW02  0x1201
#define EDIT_UNKNW03  0x1300
#define EDIT_UNKNW04  0x1400
#define EDIT_UNKNW05  0x1420
#define EDIT_UNKNW06  0x1450
#define EDIT_UNKNW07  0x1500
#define EDIT_UNKNW08  0x2200
#define EDIT_UNKNW09  0x2201
#define EDIT_UNKNW10  0x2210
#define EDIT_UNKNW11  0x2300
#define EDIT_UNKNW12  0x2302 // new chunk type
#define EDIT_UNKNW13  0x3000
#define EDIT_UNKNW14  0xAFFF

		//>------ sub defines of EDIT_OBJECT

#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

#define OBJ_UNKNWN01  0x4010
#define OBJ_UNKNWN02  0x4012 //>>---- Could be shadow

		//>------ sub defines of OBJ_TRIMESH
#define TRI_MAPPINGCOORS 0x4140
#define TRI_VERTEXL   0x4110
#define TRI_FACEL1    0x4120
#define TRI_FACEL2 0x4111
#define TRI_SMOOTH    0x4150
#define TRI_LOCAL     0x4160
#define TRI_VISIBLE   0x4165

		ifstream bin3ds;
		unsigned long current_chunk = 0L;
		string temp_name;

		bool Parse();

		unsigned char ReadChar();
		unsigned short ReadInt();
		unsigned long ReadLong();
		float ReadFloat();
		unsigned long ReadChunkPointer();
		unsigned long GetChunkPointer();
		void ChangeChunkPointer(unsigned long temp_pointer);
		int ReadName(bool isLongName);
		unsigned long ReadChunk(unsigned int chunk_id);
		bool ReadPrimaryChunk();
	};

public:
	C3DModel_3DS();
	~C3DModel_3DS();

protected:
	CVector3* m_vertices;
	CVector3* m_UVCoords;
	CVector3* m_normals;

	void reset();                                     // Cleanup any allocated memory
	bool loadFromFile(const char * const filename);   // Read object from file
};

#endif // !C3DMODEL_3DS_H