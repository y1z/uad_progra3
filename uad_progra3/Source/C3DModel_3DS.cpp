#include "../stdafx.h"
#include <iostream>
#include <sstream>
using namespace std;

#include "../Include/C3DModel_3DS.h"

/* */
C3DModel_3DS::C3DModel_3DS() : C3DModel()
{
	cout << "Constructor: C3DModel_3DS()" << endl;
}

/* */
C3DModel_3DS::~C3DModel_3DS()
{
	cout << "Destructor: C3DModel_3DS()" << endl;
	reset();
}

/* */
bool C3DModel_3DS::loadFromFile(const char * filename)
{
	C3DS_Parser parser(filename);

	if (parser.Success)
	{
		if (parser.indices.size() > 0)
		{
			this->m_vertexIndices = new unsigned short[parser.indices.size()];
			memcpy(this->m_vertexIndices, &(parser.indices)[0], parser.indices.size() * sizeof(unsigned short));

			this->m_UVindices = new unsigned short[parser.indices.size()];
			memcpy(this->m_UVindices, &(parser.indices)[0], parser.indices.size() * sizeof(unsigned short));
		}

		if (parser.verticesRaw.size() > 0)
		{
			this->m_verticesRaw = new float[parser.verticesRaw.size()];
			memcpy(this->m_verticesRaw, &(parser.verticesRaw)[0], parser.verticesRaw.size() * sizeof(float));
		}

		if (parser.UVcoordsRaw.size() > 0)
		{
			this->m_uvCoordsRaw = new float[parser.UVcoordsRaw.size()];
			memcpy(this->m_uvCoordsRaw, &(parser.UVcoordsRaw)[0], parser.UVcoordsRaw.size() * sizeof(float));
		}

		if (parser.vertices.size() > 0)
		{
			this->m_vertices = new CVector3[parser.vertices.size()];
			memcpy(this->m_vertices, &(parser.vertices)[0], parser.vertices.size() * sizeof(CVector3));
		}

		if (parser.UVcoords.size() > 0)
		{
			this->m_UVCoords = new CVector3[parser.UVcoords.size()];
			memcpy(this->m_UVCoords, &(parser.UVcoords)[0], parser.UVcoords.size() * sizeof(CVector3));
		}

		this->m_numFaces = parser.indices.size() / 3;
		this->m_numVertices = parser.vertices.size();
		this->m_numUVCoords = parser.UVcoords.size();

		computeFaceNormals();
		m_Initialized = true;

		return true;
	}

	return false;
}

/* */
void C3DModel_3DS::reset()
{
	if (m_vertexIndices != NULL)
	{
		cout << "deleting vertex indices" << endl;
		delete[] m_vertexIndices;
		m_vertexIndices = NULL;
	}
	if (m_normalIndices != NULL)
	{
		cout << "deleting normal indices" << endl;
		delete[] m_normalIndices;
		m_normalIndices = NULL;
	}
	if (m_UVindices != NULL)
	{
		cout << "deleting UV indices" << endl;
		delete[] m_UVindices;
		m_UVindices = NULL;
	}
	if (m_vertices != NULL)
	{
		cout << "deleting vertices" << endl;
		delete[] m_vertices;
		m_vertices = NULL;
	}
	if (m_verticesRaw != NULL)
	{
		cout << "deleting vertices (float)" << endl;
		delete[] m_verticesRaw;
		m_verticesRaw = NULL;
	}
	if (m_normals != NULL)
	{
		cout << "deleting normals" << endl;
		delete[] m_normals;
		m_normals = NULL;
	}
	if (m_normalsRaw != NULL)
	{
		cout << "deleting normals (float)" << endl;
		delete[] m_normalsRaw;
		m_normalsRaw = NULL;
	}
	if (m_UVCoords != NULL)
	{
		cout << "deleting UV coords" << endl;
		delete[] m_UVCoords;
		m_UVCoords = NULL;
	}
	if (m_uvCoordsRaw != NULL)
	{
		cout << "deleting uvCoords (float)" << endl;
		delete[] m_uvCoordsRaw;
		m_uvCoordsRaw = NULL;
	}

	m_numVertices = 0;
	m_numNormals = 0;
	m_numUVCoords = 0;
	m_numFaces = 0;

	m_Initialized = false;

	//m_currentVertex = 0;
	//m_currentNormal = 0;
	//m_currentUV = 0;
	//m_currentFace = 0;

	m_graphicsMemoryObjectId = 0;
	m_shaderProgramId = 0;
}

/* */
void C3DModel_3DS::computeFaceNormals()
{
	m_normals = new CVector3[m_numFaces];
	m_normalsRaw = new float[m_numFaces * 3];
	m_normalIndices = new unsigned short[m_numFaces * 3];
	for (size_t i = 0; i < m_numFaces; i++)
	{
		size_t ivert = i * 3;
		//CVector3 A = vertices[m_vertexIndices[ivert]];
		//CVector3 B = vertices[m_vertexIndices[ivert + 1]];
		//CVector3 C = vertices[m_vertexIndices[ivert + 2]];
		CVector3 A = m_vertices[m_vertexIndices[ivert]];
		CVector3 B = m_vertices[m_vertexIndices[ivert + 1]];
		CVector3 C = m_vertices[m_vertexIndices[ivert + 2]];

		m_normals[i] = (B - A).cross(C - B);
		m_normalsRaw[ivert] = m_normals[i].getX();
		m_normalsRaw[ivert + 1] = m_normals[i].getY();
		m_normalsRaw[ivert + 2] = m_normals[i].getZ();

		m_normalIndices[ivert] = i;
		m_normalIndices[ivert + 1] = i;
		m_normalIndices[ivert + 2] = i;
	}
}

/* */
C3DModel_3DS::C3DS_Parser::C3DS_Parser(string filename)
{
	indices.clear();

	verticesRaw.clear();
	UVcoordsRaw.clear();

	vertices.clear();
	UVcoords.clear();

	bin3ds.open(filename, ios::in | ios::binary);

	Success = Parse();

	bin3ds.close();
}

/* */
C3DModel_3DS::C3DS_Parser::~C3DS_Parser() {}

/* */
bool C3DModel_3DS::C3DS_Parser::Parse()
{
	if (!bin3ds.is_open()) return false;
	return ReadPrimaryChunk();
}

/* */
unsigned char C3DModel_3DS::C3DS_Parser::ReadChar()
{
	char c;
	bin3ds.read(&c, 1);
	return c;
}

/* */
unsigned short C3DModel_3DS::C3DS_Parser::ReadInt()
{
	unsigned short temp = ReadChar();
	return (temp | (ReadChar() << 8));
}

/* */
unsigned long C3DModel_3DS::C3DS_Parser::ReadLong()
{
	unsigned long temp1, temp2;

	temp1 = ReadInt();
	temp2 = ReadInt();

	return (temp1 + (temp2 * 0x10000L));
}

/* */
float C3DModel_3DS::C3DS_Parser::ReadFloat()
{
	float f;
	bin3ds.read(reinterpret_cast<char*>(&f), sizeof(float));
	return f;
}

/* */
unsigned long C3DModel_3DS::C3DS_Parser::ReadChunkPointer()
{
	return ReadLong();
}

/* */
unsigned long C3DModel_3DS::C3DS_Parser::GetChunkPointer()
{
	return ((unsigned long)bin3ds.tellg() - 2); // compensate for the already read Marker
}

/* */
void C3DModel_3DS::C3DS_Parser::ChangeChunkPointer(unsigned long temp_pointer)
{
	bin3ds.seekg(temp_pointer, ios::beg);
}

/* */
int C3DModel_3DS::C3DS_Parser::ReadName(bool isLongName)
{
	unsigned char letter;

	temp_name.clear();

	do
	{
		letter = ReadChar();
		temp_name += letter;
	} while ((letter != 0) && (temp_name.size() < 12 || isLongName));

	if (temp_name == "")
	{
		temp_name = "Default name";
		return -1;
	}

	cout << "     Found name : " << temp_name << endl;
	return 0;
}

/* */
unsigned long C3DModel_3DS::C3DS_Parser::ReadChunk(unsigned int chunk_id)
{
	unsigned int temp_int;
	unsigned long current_pointer;
	unsigned long chunk_size;//temp_pointer
	unsigned long counter = 6L;

	current_pointer = GetChunkPointer();
	chunk_size = ReadChunkPointer();

	if (chunk_id == EDIT_OBJECT) ReadName(false);
	if (chunk_id == TRI_VERTEXL)
	{
		unsigned int numb_vertices = ReadInt();
		cout << "      Found (" << numb_vertices << ") number of vertices" << endl;

		for (size_t i = 0; i < numb_vertices; i++)
		{
			CVector3 vertex;

			verticesRaw.push_back(ReadFloat()); //***
			verticesRaw.push_back(ReadFloat());
			verticesRaw.push_back(ReadFloat());

			auto it = verticesRaw.rbegin();
			vertex.setValues(it[2], it[1], it[0]);
			vertices.push_back(vertex); //***
		}

		counter = chunk_size;
	}
	if (chunk_id == TRI_FACEL1)
	{
		unsigned int numb_faces = ReadInt();
		cout << "      Found (" << numb_faces << ") number of faces" << endl;

		for (size_t i = 0; i < numb_faces; i++)
		{
			indices.push_back(ReadInt());
			indices.push_back(ReadInt());
			indices.push_back(ReadInt());
			ReadInt();
		}

		if (ReadInt() == TRI_SMOOTH)
			ReadChunk(TRI_SMOOTH);

		counter = chunk_size;
	}
	if (chunk_id == TRI_MAPPINGCOORS)
	{
		unsigned short numVertices = ReadInt();
		cout << "      Found (" << numVertices << ") number of uv vertices" << endl;
		for (size_t i = 0; i < numVertices; i++)
		{
			CVector3 vertex;

			UVcoordsRaw.push_back(ReadFloat());
			UVcoordsRaw.push_back(ReadFloat());

			auto it = UVcoordsRaw.rbegin();
			vertex.setValues(it[1], it[0], -1);
			UVcoords.push_back(vertex); // ***
		}
		counter = chunk_size;
	}


	while (counter < chunk_size)
	{
		temp_int = ReadInt();
		switch (chunk_id)
		{
		case MAIN3DS: //ReadMainChunk
			switch (temp_int)
			{
			case KEYF3DS:
				cout << ">> Found *Keyframer* chunk id of " << std::hex << KEYF3DS << std::dec << endl;
				counter += ReadChunk(KEYF3DS);
				break;
			case EDIT3DS:
				cout << ">> Found *Editor* chunk id of " << std::hex << EDIT3DS << std::dec << endl;
				counter += ReadChunk(EDIT3DS);
				break;
			default: break;
			}
			break;
		case EDIT3DS://ReadEditChunk
			switch (temp_int)
			{
			case EDIT_UNKNW01:counter += ReadChunk(EDIT_UNKNW01); break;
			case EDIT_UNKNW02:counter += ReadChunk(EDIT_UNKNW02); break;
			case EDIT_UNKNW03:counter += ReadChunk(EDIT_UNKNW03); break;
			case EDIT_UNKNW04:counter += ReadChunk(EDIT_UNKNW04); break;
			case EDIT_UNKNW05:counter += ReadChunk(EDIT_UNKNW05); break;
			case EDIT_UNKNW06:counter += ReadChunk(EDIT_UNKNW06); break;
			case EDIT_UNKNW07:counter += ReadChunk(EDIT_UNKNW07); break;
			case EDIT_UNKNW08:counter += ReadChunk(EDIT_UNKNW08); break;
			case EDIT_UNKNW09:counter += ReadChunk(EDIT_UNKNW09); break;
			case EDIT_UNKNW10:counter += ReadChunk(EDIT_UNKNW10); break;
			case EDIT_UNKNW11:counter += ReadChunk(EDIT_UNKNW11); break;
			case EDIT_UNKNW12:counter += ReadChunk(EDIT_UNKNW12); break;
			case EDIT_UNKNW13:counter += ReadChunk(EDIT_UNKNW13); break;
			case EDIT_MATERIAL: counter += ReadChunk(EDIT_MATERIAL); break;
			case EDIT_VIEW1: counter += ReadChunk(EDIT_VIEW1); break;
			case EDIT_BACKGR: counter += ReadChunk(EDIT_BACKGR); break;
			case EDIT_AMBIENT: counter += ReadChunk(EDIT_AMBIENT); break;
			case EDIT_OBJECT: counter += ReadChunk(EDIT_OBJECT); break;
			default: break;
			}
			break;
		case EDIT_OBJECT://ReadObjectChunk
			switch (temp_int)
			{
			case OBJ_UNKNWN01:counter += ReadChunk(OBJ_UNKNWN01); break;
			case OBJ_UNKNWN02:counter += ReadChunk(OBJ_UNKNWN02); break;
			case OBJ_TRIMESH:
				cout << ">>>> Found Obj/Mesh chunk id of " << std::hex << OBJ_TRIMESH << std::dec << endl;
				counter += ReadChunk(OBJ_TRIMESH);
				break;
			case OBJ_LIGHT: counter += ReadChunk(OBJ_LIGHT); break;
			case OBJ_CAMERA: counter += ReadChunk(OBJ_CAMERA); break;
			default:          break;
			}
			break;
		case OBJ_TRIMESH://ReadObjChunk
			switch (temp_int)
			{
			case TRI_VERTEXL:
				cout << ">>>> Found Object vertices chunk id of " << std::hex << TRI_VERTEXL << std::dec << endl;
				counter += ReadChunk(TRI_VERTEXL);
				break;
			case TRI_FACEL1:
				cout << ">>>> Found Object faces (1) chunk id of " << std::hex << TRI_FACEL1 << std::dec << endl;
				counter += ReadChunk(TRI_FACEL1);
				break;
			case TRI_MAPPINGCOORS:
				cout << ">>>> Found Mapping Coordinates chunk id of " << std::hex << TRI_MAPPINGCOORS << std::dec << endl;
				counter += ReadChunk(TRI_MAPPINGCOORS);
				break;

			case TRI_FACEL2: counter += ReadChunk(TRI_FACEL2); break;
			case TRI_LOCAL: counter += ReadChunk(TRI_LOCAL); break;
			case TRI_VISIBLE: counter += ReadChunk(TRI_VISIBLE); break;
			default: break;
			}
			break;
		default:
			counter = chunk_size;
			break;
		}

		counter += 2;
	}

	ChangeChunkPointer(current_pointer + chunk_size);
	// move to the new chunk position
	return chunk_size;
}

/* */
bool C3DModel_3DS::C3DS_Parser::ReadPrimaryChunk()
{
	unsigned char version;

	if (ReadInt() == MAIN3DS)
	{
		cout << "> Found Main chunk id of " << std::hex << MAIN3DS << std::dec << endl;
		bin3ds.seekg(28L, ios::beg);
		version = ReadChar();
		if (version < 3)
		{
			cout << "Sorry this lib can only read 3ds files of version 3.0 and higher" << endl;
			cout << "The version of the file you want to read is: " << version << endl;
			return true;
		}
		bin3ds.seekg(2, ios::beg);
		ReadChunk(MAIN3DS);
		return true;
	}
	return false;
}