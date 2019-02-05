#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "../Include/CAppGeometricFigures.h"
#include "../Include/CWideStringHelper.h"

/* */
CAppGeometricFigures::CAppGeometricFigures() :
	CAppGeometricFigures(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppGeometricFigures::CAppGeometricFigures(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ -1.5f, 0.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_pyramidVertexArrayObject{ 0 },
	m_numFacesPyramid{ 0 },
	m_renderPolygonMode{ 0 }
	
{
	cout << "Constructor: CAppGeometricFigures(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppGeometricFigures::~CAppGeometricFigures()
{
	cout << "Destructor: ~CAppGeometricFigures()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	if (m_textureID > 0)
	{
		getOpenGLRenderer()->deleteTexture(&m_textureID);
	}

	if (m_pyramidVertexArrayObject > 0)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
	}
	// =================================================
}

/*TODO : Agregar mis figuras geometircas aqui 
*/
void CAppGeometricFigures::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS,resourceFilenameFS, resourceFilenameTexture;

	// Color Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the color-only object exists
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_colorModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	// Texture Shader
	// Load shader file, create OpenGL Shader Object for it
	// -------------------------------------------------------------------------------------------------------------

	// Check shader for the textured object exists
	wresourceFilenameFS.clear();
	wresourceFilenameVS.clear();
	resourceFilenameFS.clear();
	resourceFilenameVS.clear();
	if (!CWideStringHelper::GetResourceFullPath(VERTEX_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(FRAGMENT_SHADER_TEXTURED_3D_OBJECT, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << VERTEX_SHADER_TEXTURED_3D_OBJECT << endl;
		cout << "  " << FRAGMENT_SHADER_TEXTURED_3D_OBJECT << endl;

		return;
	}

	if (!getOpenGLRenderer()->createShaderProgram(
		&m_texturedModelShaderId,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_LEAVES_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	// Initialize the texture
	m_textureID = 0;
	if (!loadTexture(resourceFilenameTexture.c_str(), &m_textureID))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}
	// TODO : Agregar mis metodos aqui 
	m_initialized = true;

	ParserFBX("MODELS\\FBX\\Pyro_Head_Ascii.fbx");

	//createTri();
	//createSphereGeometry(6, 6, 1.f);
	//createPyramidGeometry();
}

/* */
void CAppGeometricFigures::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();

				// Enter main loop
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}
		}
	}
}

/* */
void CAppGeometricFigures::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	double degreesToRotate = 0.0;

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
	// ===============================
}

/* */
void CAppGeometricFigures::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		// White 
		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;
	
		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, m_objectPosition);

		if (m_pyramidVertexArrayObject > 0 && m_numFacesPyramid > 0)
		{
			CVector3 pos2 = m_objectPosition;
			pos2 += CVector3(3.0f, 0.0f, 0.0f);
			MathHelper::Matrix4 modelMatrix2 = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, pos2);

			// Render pyramid in the first position, using the color shader
			getOpenGLRenderer()->renderObject(
				&m_colorModelShaderId,
				&m_pyramidVertexArrayObject,
				&noTexture,
				m_numFacesPyramid,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);

			// Render same pyramid (same vertex array object identifier), in a second position, but this time with a texture
			getOpenGLRenderer()->renderObject(
				&m_texturedModelShaderId,
				&m_pyramidVertexArrayObject,
				&m_textureID,
				m_numFacesPyramid,
				color,
				&modelMatrix2,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}
		// =================================
	}
}
/* */
void CAppGeometricFigures::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}
/* */
void CAppGeometricFigures::createPyramidGeometry()
{
	float height = 2.25f;
	float sideHalfX = 0.75f;
	float sideHalfZ = 1.0f;
	bool loaded = false;
	float v1[3], v2[3], v3[3], v1v2[3], v1v3[3], norm[3];

	float vData[15] = {
		0.0, height, 0.0,                // TOP
		-sideHalfX,  0.0,   sideHalfZ,   // BOTTOM LEFT, FRONT
	 	 sideHalfX,  0.0,   sideHalfZ,   // BOTTOM RIGHT, FRONT
		-sideHalfX,  0.0,  -sideHalfZ,   // BOTTOM LEFT, BACK
		 sideHalfX,  0.0,  -sideHalfZ    // BOTTOM RIGHT, BACK
	};

	float vertexUVs[10] =
	{
		0.5f,  0.11f, // TOP 
		0.25f, 0.99f, // BOTTOM LEFT, FRONT
		0.75f, 0.99f, // BOTTOM RIGHT, FRONT
		0.11f, 0.40f, // BOTTOM LEFT, BACK
		0.99f, 0.40f  // BOTTOM RIGHT, BACK 
	};

	m_numFacesPyramid = 6;
	/*La puta 't' significa 
	 TRIS */
	unsigned short tIndices[18] = {
		0, 1, 2,    
		0, 2, 4,    
		0, 4, 3,    
		0, 3, 1,    
		1, 3, 2,
		2, 3, 4
	};
	// son para los shaders 
	float nData[18] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};

	unsigned short nIndices[18] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2,
		3, 3, 3,
		4, 4, 4,
		5, 5, 5
	};

	for (int i = 0; i < m_numFacesPyramid; i++)
	{
		// Vertex 1
		v1[0] = vData[tIndices[i * 3]];
		v1[1] = vData[tIndices[i * 3] + 1];
		v1[2] = vData[tIndices[i * 3] + 2];

		// Vertex 2
		v2[0] = vData[tIndices[(i * 3) + 1]];
		v2[1] = vData[tIndices[(i * 3) + 1] + 1];
		v2[2] = vData[tIndices[(i * 3) + 1] + 2];

		// Vertex 3
		v3[0] = vData[tIndices[(i * 3) + 2]];
		v3[1] = vData[tIndices[(i * 3) + 2] + 1];
		v3[2] = vData[tIndices[(i * 3) + 2] + 2];

		// Vector from v2 to v1
		v1v2[0] = v1[0] - v2[0];
		v1v2[1] = v1[1] - v2[1];
		v1v2[2] = v1[2] - v2[2];

		// Vector from v2 to v3
		v1v3[0] = v3[0] - v2[0];
		v1v3[1] = v3[1] - v2[1];
		v1v3[2] = v3[2] - v2[2];

		normcrossprod(v1v2, v1v3, norm);

		nData[i * 3]       = norm[0];
		nData[(i * 3) + 1] = norm[1];
		nData[(i * 3) + 2] = norm[2];
	}
	
	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		vData,
		5,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		nData,     
		6,
		vertexUVs,
		5,
		tIndices,
		6,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		nIndices,
		6,
		tIndices,
		6
	);
		
	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
}
// TODO : Crear la Geometria para la esphera 
void CAppGeometricFigures::createSphereGeometry(unsigned int PointsX, unsigned int PointsY, float Size){
	float RadioY = 1.0f * Size;	
	float RadioX = 1.0f * Size;
	float z = 1.0f * Size;
	
	const float FullCircleRadians = 2 * (PI * RadioX);	

	float AngleX = FullCircleRadians / PointsX;
	float AngleY = FullCircleRadians / PointsY;

	CVector3 Test(RadioX, RadioY, z);

	float Debugtemp1 = 0.f;
	float Debugtemp2 = 0.f;

	for (int i = 0; i < PointsX; ++i)
	{
		Debugtemp1 = (Test.X * cos(AngleX)) + (Test.Y * -sin(AngleY));
		Debugtemp2 = (Test.Y * cos(AngleY)) + (Test.X * sin(AngleX)) ;
		AngleX +=FullCircleRadians / PointsX;
		AngleY +=FullCircleRadians / PointsY;
	}

 	CVector3 Points(RadioX, RadioY, z);
	float *ptr_points = new float[PointsX];

	for (int i = 0; i < PointsX / 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			MathHelper::ModelMatrix;
		
		}
		
	}
	
	delete[]ptr_points;
}

// TODO : Crear la geometria para la dona 
void CAppGeometricFigures::createTorusGeometry(){

}
// TODO : Crear la geometria para el Icsaedro 
void CAppGeometricFigures::createIcsaedroGeometry()
{

}

void CAppGeometricFigures::createTri(){

	m_numFacesPyramid = 1;

	// Posiciones x,y,z
	float vertixData[9] = 
	{
		0.f,0.f,0.f,
		0.5f,1.f,0.f,
		1.f,0.f,0.f
	};
	/*El orden en el que se van a 
	connectar los puntos */
	unsigned short IndicesData[3] =
	{
		0,2,1
	};

	/*para las normales */
	float nData[3] = {
		0.0, 0.0, 0.0,
	};

	/*para las indices de los normales */
	unsigned short nIndices[3] = {
		0, 0, 0,
	};
	/*Cordenadas uv de cada triangulo */
	float uvVertex[2] = {
		0.0f,0.0f
	};

	unsigned short uvCoords[6] = {
		0,0,
		0,0,
		0,0
	};

	bool loaded = false;

	// Allocate graphics memory for object
	loaded =  getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		vertixData,3,//
		nData,1,//
		uvVertex,1,
		IndicesData,1,
		nIndices,1,
		uvCoords,3
	);
	
	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}
}

/*!
	\fn Takes a relative path to an FBX file then parser 
	\param NameOfFile is the locaction of the file that goin to be parse
*/
void CAppGeometricFigures::ParserFBX(const char* PathOfFile)
{
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	if (!CWideStringHelper::GetResourceFullPath(PathOfFile
		, wresourceFilenameTexture, resourceFilenameTexture)) 
	{
		std::cout << "ERROR : With path ";
	}

	/*! \var ComparasionStr is use to search for tokens */
	string ComparasionStr;
	string ResultStr;
	/*! \var To know when we find the part of the file we need */
	bool isInformationRelevant = false;

	ifstream File(resourceFilenameTexture.c_str());

	int CountProperty = 0;
	int CountVertices = 0;
	int CountPolygonVertex = 0;
	int CountEdges = 0;
	int CountNormals = 0;
	int CountUVs = 0;
	int CountUVIndex = 0;


	float *ptr_Vertices = nullptr;
	float *ptr_Normal = nullptr;
	float*ptr_UV = nullptr;

	 short *ptr_Polygon = nullptr;
	unsigned short *ptr_UVindex = nullptr;

	if(File.is_open())
	{
		char temp = '\0';
		// this token we tell us wen we get to the other tokens 
		const char *TokenObjects = "Objects:";
		// all the properties of the 3d objets we are parsing
		const char *TokenVertices = "Vertices:";
		const char *TokenPolygonVertexIndex = "PolygonVertexIndex:";
		const char *TokenEdges = "Edges:";
		const char *TokenNormals = "Normals:";
		const char *TokenUV = "UV:";
		const char *TokenUVIndex = "UVIndex:";


		/*! where the value are going to be stored */

		uint8_t LengthOfToken = 0;
		
		bool HaveUV = false;
		do
		{
			File >> temp;
			if(!isInformationRelevant && temp == 'O')
			{
				LengthOfToken = strlen(TokenObjects);
				for (size_t i = 0; i <LengthOfToken; i++)
				{
					ComparasionStr += temp;
					File >> temp;
				}
				if (ComparasionStr == TokenObjects)
				{
					isInformationRelevant = true;
				}
				// This else is for skiping the line
				// a get to the next one 
				else{
					getline(File, ComparasionStr);
				}
			}
			else if(isInformationRelevant)
			{
				bool GoodToken = false;
				if(temp == 'V')
				{
					GoodToken =	isTokenValid(File, ComparasionStr, TokenVertices,temp);
					if (GoodToken)
					{
						GetValueFromStr(File, ptr_Vertices, CountVertices);
					}
				}
				else if(temp == 'N')
				{
					GoodToken = isTokenValid(File, ComparasionStr, TokenNormals, temp);
					if (GoodToken)
					{
						GetValueFromStr(File, ptr_Normal, CountNormals);
					}
				}
				else if (temp == 'P')
				{
					GoodToken = isTokenValid(File, ComparasionStr, TokenPolygonVertexIndex, temp);
					if (GoodToken)
					{
						GetValueFromStr(File, ptr_Polygon, CountPolygonVertex);
					}
				}
				else if(temp == 'U' && !HaveUV)
				{
					GoodToken = isTokenValid(File, ComparasionStr, TokenUV, temp);
					if (GoodToken)
					{
						GetValueFromStr(File, ptr_UV, CountUVs);
						HaveUV = true;
					}
				}
				else if(HaveUV && temp == 'U')
				{
					GoodToken = isTokenValid(File, ComparasionStr, TokenUVIndex, temp);
					if (GoodToken)
					{
						GetValueFromStr(File, ptr_UVindex, CountUVIndex);
					}
				}
				else
				{
					getline(File, ComparasionStr);
				}

			}
			ComparasionStr.clear();
		} while (!File.eof());
	}
	else {
		std::cout << "ERROR : cannot find File\n";
	}

	File.close();
	bool loaded = false;

	m_numFacesPyramid = CountPolygonVertex / 3;

	unsigned short *ptr_FakeNormalIncies = new 	unsigned short[CountNormals];
	short indexCount = 1;
	for (int i = 0; i < CountNormals; ++i)
	{
		ptr_FakeNormalIncies[i] = indexCount;
		if(i % 3 ==0 && i > 1)
		{
			indexCount++;
		}

	}

	for (int i = 0; i < CountPolygonVertex; ++i)
	{
		if (ptr_Polygon[i] < 0)
		{
			ptr_Polygon[i] =abs(ptr_Polygon[i]);
			ptr_Polygon[i] -= 1 ;
		}
	}

	 unsigned short * ptr_Trasfer = new unsigned short[CountPolygonVertex];

	 for(int i = 0; i < CountPolygonVertex; ++i)
	 {
		 ptr_Trasfer[i] = ptr_Polygon[i];
	 }


	// Allocate graphics memory for object
	loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_pyramidVertexArrayObject,
		ptr_Vertices, CountVertices / 3,//
		ptr_Normal, CountNormals / 3,//
		ptr_UV, CountUVs / 2 ,
		ptr_Trasfer, CountPolygonVertex / 3,
		ptr_FakeNormalIncies, CountNormals / 3,
		ptr_UVindex, CountUVIndex / 3
	);

	if (!loaded)
	{
		m_numFacesPyramid = 0;

		if (m_pyramidVertexArrayObject > 0)
		{
			getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_pyramidVertexArrayObject);
			m_pyramidVertexArrayObject = 0;
		}
	}

	if (ptr_Vertices != nullptr)
	{
		delete[] ptr_Vertices;
	}
	if (ptr_Normal != nullptr)
	{
		delete[] ptr_Normal;
	}
	if (ptr_UV != nullptr)
	{
		delete[]ptr_UV;
	}
	if (ptr_UVindex != nullptr)
	{
		delete[] ptr_UVindex;
	}
	if (ptr_Polygon != nullptr)
	{
		delete[] ptr_Polygon;
	}
	if (ptr_Trasfer != nullptr)
	{
		delete[] ptr_Trasfer;
	}
	if (ptr_FakeNormalIncies != nullptr)
	{
		delete[] ptr_FakeNormalIncies;
	}
}

bool CAppGeometricFigures::isTokenValid(ifstream &inFile, string &Str, const char *Token, char CurrentChr)
{
	uint16_t TokenLength = strlen(Token);

	for (int i = 0; i < TokenLength; ++i)
	{
		Str += CurrentChr;
		inFile >> CurrentChr;
	}

	if (Str == Token)
		return true;

	return false;
}
// get Value Fuction
/*!
	\fn GetValueFromStr This Function WILL alter the pointer thats past to it 
	\param inFile the file for input
	\param ptr_float a reference to a pointer 
	\param Count to know how many parperty iva parsed 
*/
void CAppGeometricFigures::GetValueFromStr(ifstream &inFile, float *&ptr_float,int &Count)
{
	char temp = '0';
	string NumReader;
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// ignore the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_float = new float[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_float[index] = stof(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_float[index] = stof(NumReader);
		}
	}

}

// sobre carga 
void CAppGeometricFigures::GetValueFromStr(ifstream &inFile,unsigned short *&ptr_Ushort, int &Count)
{
	char temp = '0';
	string NumReader;
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// ignore the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_Ushort = new unsigned short[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_Ushort[index] = stoi(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_Ushort[index] = stoi(NumReader);
		}
	}

}

void CAppGeometricFigures::GetValueFromStr(ifstream &inFile, short *&ptr_short, int &Count)
{
	char temp = '0';
	string NumReader;
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// ignore the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_short = new short[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_short[index] = stoi(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_short[index] = stoi(NumReader);
		}
	}

}
/* */
void CAppGeometricFigures::onF2(int mods)
{
}

/* */
void CAppGeometricFigures::onF3(int mods)
{
	if (m_renderPolygonMode == 0)
	{
		getOpenGLRenderer()->setFillPolygonMode();
		m_renderPolygonMode = 1;
	}
	else
	{
		getOpenGLRenderer()->setWireframePolygonMode();
		m_renderPolygonMode = 0;
	}
}

/* */
void CAppGeometricFigures::normcrossprod(float v1[3], float v2[3], float out[3])
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];

	normalize(out);
}

/* */
void CAppGeometricFigures::normalize(float v[3])
{
	float d = sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0f)
	{
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}