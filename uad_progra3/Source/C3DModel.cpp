#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../Include/C3DModel.h"
#include "../Include/C3DModel_Obj.h"
#include "../Include/C3DModel_STL.h"
#include "../Include/C3DModel_3DS.h"

/* */
C3DModel::C3DModel()
	: m_vertexIndices(NULL),
	m_normalIndices(NULL),
	m_UVindices(NULL),
	m_verticesRaw(NULL),
	m_normalsRaw(NULL),
	m_uvCoordsRaw(NULL),
	m_Initialized(false),
	m_numVertices(0), 
	m_numNormals(0), 
	m_numUVCoords(0), 
	m_numFaces(0),
	m_graphicsMemoryObjectId(0),
	m_shaderProgramId(0),
	m_textureObjectId(0),
	m_modelHasNormals(false),
	m_modelHasUVs(false),
	m_modelHasTextures(false),
	m_modelTextureFilename(nullptr)
{
	cout << "Constructor: C3DModel()" << endl;
}

/* */
C3DModel::~C3DModel()
{
	cout << "Destructor: C3DModel()" << endl;
	reset();
}

/*
*/
void C3DModel::reset()
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
	if (m_verticesRaw != NULL)
	{
		cout << "deleting vertices (float)" << endl;
		delete[] m_verticesRaw;
		m_verticesRaw = NULL;
	}
	if (m_normalsRaw != NULL)
	{
		cout << "deleting normals (float)" << endl;
		delete[] m_normalsRaw;
		m_normalsRaw = NULL;
	}
	if (m_uvCoordsRaw != NULL)
	{
		cout << "deleting uvCoords (float)" << endl;
		delete[] m_uvCoordsRaw;
		m_uvCoordsRaw = NULL;
	}
	if (m_modelTextureFilename != nullptr)
	{
		delete[] m_modelTextureFilename;
		m_modelTextureFilename = nullptr;
	}

	m_numVertices = 0;
	m_numNormals = 0;
	m_numUVCoords = 0;
	m_numFaces = 0;
	
	m_Initialized = false;

	m_graphicsMemoryObjectId = 0;
	m_shaderProgramId = 0;
	m_textureObjectId = 0;
	m_modelHasNormals = false;
	m_modelHasUVs = false;
	m_modelHasTextures = false;
}

/*
 Static method ("abstract method" pattern) that checks the filename and returns a new object of the appropriate subclass
*/
C3DModel* C3DModel::load(const char * const filename)
{
	C3DModel *newModel = nullptr;

	// Check the file type
	// We could use the "PathFindExtension" function but that needs the shlwapi.lib, instead we'll keep it simple and avoid more dependencies
	std::string stdFilename(filename);
	size_t dotIndex = stdFilename.rfind('.', stdFilename.length());
	if (dotIndex != string::npos)
	{
		std::string fileExtension = stdFilename.substr(dotIndex + 1, stdFilename.length() - dotIndex);

		// Convert to lowercase
		// NOTE: ::tolower works on single bytes, which can be a problem for multi-byte encoding, like UTF8
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

		cout << "File extension: " << fileExtension << endl;

		// Now check the file type and see if it's a supported type
		if (!fileExtension.compare("obj"))
		{
			cout << "Loading OBJ model..." << endl;
			newModel = new C3DModel_Obj();
			newModel->loadFromFile(filename);
		}
		else if (!fileExtension.compare("3ds"))
		{
			cout << "Loading 3DS model..." << endl;
			newModel = new C3DModel_3DS();
			newModel->loadFromFile(filename);
		}
		else if (!fileExtension.compare("stl"))
		{
			cout << "Loading STL model..." << endl;
			newModel = new C3DModel_STL();
			newModel->loadFromFile(filename);
		}
		else if (!fileExtension.compare("fbx"))
		{
			cout << "FBX file format reading not implemented" << endl;
		}
	}
	else
	{
		cout << "ERROR: Cannot determine the file type" << endl;
		return nullptr;
	}

	return newModel;
}

/*
*/
void C3DModel::computeFaceNormals()
{
	// Calculate face normals
	// ...

	// After calculating the normals, set the flag to true
	// m_modelHasNormals = true;
}