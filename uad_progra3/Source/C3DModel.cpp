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
	m_modelHasTextureFilename(false),
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
	m_modelHasTextureFilename = false;
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
/*
bool C3DModel::loadFromFile(const char * const filename)
{
	bool readFileOk = false;
	bool modelNeedsNormals = false;

	// Free any previous resources
	reset();
	
	

	// First pass is to count the number of vertices, normals, UVs, faces
	readFileOk = readObjFile(filename, true);

	// Display count
	cout << "Finished reading 3D model" << endl;
	cout << "Vertices: " << m_numVertices << endl;
	cout << "Normals: " << m_numNormals << endl;
	cout << "UVCoords: " << m_numUVCoords << endl;
	cout << "Faces: " << m_numFaces << endl;

	if (readFileOk)
	{
		// Check for MAX number of faces
		if (m_numVertices >= 65535 || m_numNormals >= 65535 || m_numUVCoords >= 65535)
		{
			cout << "Error: object cannot have more than 65535 vertices" << endl;
			cout << "Object attempted to load has: " << m_numVertices << " vertices" << endl;
			return false;
		}

		// If model was read without normals or UVCoords, we'll set a default value for them
		// i.e.:
		//   0,0 for UV coords
		//   face normal for normal
		if (m_numNormals == 0)
		{
			modelNeedsNormals = true;
			m_numNormals = m_numVertices;
		}
		if (m_numUVCoords == 0)
		{
			m_numUVCoords = m_numVertices;
		}

		// Allocate memory for the arrays
		m_vertices      = new CVector3[m_numVertices];
		m_verticesRaw   = new float[m_numVertices * 3];
		m_normals       = new CVector3[m_numNormals];
		m_normalsRaw    = new float[m_numNormals * 3];
		m_UVCoords      = new CVector3[m_numUVCoords];
		m_uvCoordsRaw   = new float[m_numUVCoords * 2];
		m_vertexIndices = new unsigned short[m_numFaces * 3];
		m_normalIndices = new unsigned short[m_numFaces * 3];
		m_UVindices     = new unsigned short[m_numFaces * 3];

		// Zero-out indices arrays
		memset(m_vertexIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_normalIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_UVindices,     0, sizeof(unsigned short) * m_numFaces * 3);
		memset(m_verticesRaw,   0, sizeof(float) * m_numVertices * 3);
		memset(m_normalsRaw,    0, sizeof(float) * m_numNormals * 3);
		memset(m_uvCoordsRaw,   0, sizeof(float) * m_numUVCoords * 2);

		// Second pass is to read the data
		readFileOk = readObjFile(filename, false);

		if (readFileOk)
		{
			m_Initialized = true;

			if (modelNeedsNormals)
			{
				computeFaceNormals();
			}
		}
	}
	else
	{
		cout << "Error ocurred while reading 3D model." << endl;
	}

	return readFileOk;
}*/

/*
*/
/*
bool C3DModel::readObjFile(const char * filename, bool countOnly)
{
	ifstream infile;
	string lineBuffer;
	bool readFileOk = true;
	int lineNumber = 0;

	infile.open(filename);

	while (!infile.eof())
	{
		getline(infile, lineBuffer);
		lineNumber++;

		if (!(this->parseObjLine(lineBuffer, countOnly, lineNumber)))
		{
			readFileOk = false;
			break;
		}
		// cout << lineBuffer << endl;
	}

	infile.close();

	return readFileOk;
}*/

/*
 * NOTE: This code reads the .obj file format and can skip normal/UV coords information if the file doesn't have it, 
 * 
 * BUT !
 * 
 * The renderer code assumes the object will have vertices AND normals AND UV coords. So this code or the renderer code needs to be updated for the case
 * where the object doesn't have them...
 * TO-DO...
 * Also, this reads files with triangles, not quads. This is also a TO-DO...
*/
/*
bool C3DModel::parseObjLine(std::string line, bool countOnly, int lineNumber)
{
	bool parsed = false;
	bool unrecognizedLine = false;

	bool readingVertex = false;
	bool readingNormal = false;
	bool readingUV = false;
	bool readingFace = false;

	char *nextToken = NULL;
	char *token = NULL;

	const char *delimiterToken = " \t";
	const char *delimiterFace = "/";

	int currentToken = 0;
	int numTokens = 0;
	int numExpectedTokens = 4;

	std::vector<std::string> tokens;

	token = strtok_s((char *)line.c_str(), delimiterToken, &nextToken);

	// Line without space or tab separators (most likely a null string)
	if (token == NULL)
	{
		parsed = true;
	}

	// If there are any tokens left
	while (token != NULL)
	{
		// If first token
		if (currentToken == 0)
		{
			// Vertex
			if (0 == strcmp(token, "v"))
			{
				readingVertex = true;

				if (countOnly)
				{
					m_numVertices++;
				}
			}
			// Normal
			else if (0 == strcmp(token, "vn"))
			{
				readingNormal = true;

				if (countOnly)
				{
					m_numNormals++;
				}
			}
			// Vertex texture
			else if (0 == strcmp(token, "vt"))
			{
				readingUV = true;

				if (countOnly)
				{
					m_numUVCoords++;
				}
			}
			// Face
			else if (0 == strcmp(token, "f"))
			{
				readingFace = true;

				if (countOnly)
				{
					// Check if this line is a quad or a triangle
					std::string nextStrToken(nextToken);

					m_numFaces++;
				}
			}
			else
			{
				// Unrecognized line
				if (countOnly)
				{
					cout << "Ignoring line #" << lineNumber << ": " << line << endl;
				}
				unrecognizedLine = true;
			}

			// If this line has an unrecognized format
			// OR If we're in count only mode and we processed the first token, 
			// break to exit method and go to next line
			if (countOnly || unrecognizedLine)
			{
				return true;
			}
		}
		else
		{
			// Add token to vector
			tokens.push_back(std::string(token));
		}

		// Read next token
		token = strtok_s(NULL, delimiterToken, &nextToken);
		currentToken++;

		// No more tokens
		if (token == NULL)
		{
			//  If we're NOT in "count only" mode
			if(!countOnly)
			{
				// Verify we have the expected number of tokens
				if (currentToken != numExpectedTokens)
				{
					cout << "Ignoring line, number of tokens doesn't match the expected." << endl;
					cout << line.c_str() << endl;
				}
				else
				{
					// Create the vertex|normal|UVcoord|face
					if (readingVertex || readingNormal || readingUV)
					{
						// Add the 3 effective tokens to the appropriate array
						float values[3];
						values[0] = (float)atof(tokens[0].c_str());
						values[1] = (float)atof(tokens[1].c_str());
						values[2] = (float)atof(tokens[2].c_str());
						parsed = true;

						if (readingVertex && m_vertices != NULL &&
							(m_currentVertex < m_numVertices) && m_numVertices > 0)
						{
							m_vertices[m_currentVertex].setValues(values);

							m_verticesRaw[(m_currentVertex * 3)]     = values[0];
							m_verticesRaw[(m_currentVertex * 3) + 1] = values[1];
							m_verticesRaw[(m_currentVertex * 3) + 2] = values[2];

							m_currentVertex++;
						}
						else if (readingNormal && m_normals != NULL &&
							(m_currentNormal < m_numNormals) && m_numNormals > 0)
						{
							m_normals[m_currentNormal].setValues(values);

							m_normalsRaw[(m_currentNormal * 3)]     = values[0];
							m_normalsRaw[(m_currentNormal * 3) + 1] = values[1];
							m_normalsRaw[(m_currentNormal * 3) + 2] = values[2];

							m_currentNormal++;
						}
						else if (readingUV && m_UVCoords != NULL &&
							(m_currentUV < m_numUVCoords) && m_numUVCoords > 0)
						{
							m_UVCoords[m_currentUV].setValues(values);

							m_uvCoordsRaw[(m_currentUV * 2)]     = values[0];
							m_uvCoordsRaw[(m_currentUV * 2) + 1] = values[1];

							m_currentUV++;
						}
						else
						{
							parsed = false;
						}
					}
					else if (
						readingFace 
						&& m_vertexIndices != NULL 
						&& m_normalIndices != NULL 
						&& m_UVindices != NULL 
						&& (m_currentFace < (m_numFaces * 3)) 
						&& m_numFaces > 0
						)
					{
						// Parse the current token to extract the vertex|normal|UV index
						parsed = true;
						nextToken = NULL;
						token = NULL;

						// Loop through all 3 groups of indices, i.e:    1/3/4    3/5/2    2/3/1
						//
						// token[0] = 1/3/4
						// token[1] = 3/5/2
						// token[2] = 2/3/1
						for (int i = 0; i < 3 && i < tokens.size(); i++)
						{
							currentToken = -1;

							// Get group of indices and split it into tokens with '/' as delimiter
							token = strtok_s((char *)tokens[i].c_str(), delimiterFace, &nextToken);

							while (nextToken != NULL && *nextToken != '\0')
							{
								currentToken++;

								if (token != NULL)
								{
									switch(currentToken)
									{
									case 0:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_vertexIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 1:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_UVindices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 2:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_normalIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									}
								}

								token = strtok_s(NULL, delimiterFace, &nextToken);
								
								// Last token
								if (token != NULL && 
									(nextToken == NULL || (nextToken != NULL && *nextToken == '\0'))
									)
								{
									currentToken++;

									switch (currentToken)
									{
									case 0:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_vertexIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 1:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_UVindices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									case 2:
										// Indices in .obj format start at 1, but our arrays start from index 0
										m_normalIndices[m_currentFace + i] = (unsigned short)(atoi(token)) - 1;
										break;
									}
								}
							} // while (nextToken != NULL)
						} // for

						m_currentFace += 3;
						token = NULL;
						nextToken = NULL;
					} // reading face

				} // reading
			} // !count only
		} // No more tokens
	}

	return parsed;
}
*/

/*
*/
void C3DModel::computeFaceNormals()
{
	// Calculate face normals
	// ...

	// After calculating the normals, set the flag to true
	// m_modelHasNormals = true;
}