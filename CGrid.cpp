#include "CGrid.h"

CGrid::CGrid(COpenGLRenderer * ptr_renderer, CGameWindow * ptr_GameWindow,
						 uint32_t HexCount, CAppHexgrid *HexGrid)
	:mptr_Render(ptr_renderer), mptr_GameWindow(ptr_GameWindow), m_CountHexagon(HexCount)
{
	mptr_hexGrid = HexGrid;
	m_objectRotation = 0.1;
}

CGrid::CGrid()
{
	m_objectRotation = 0.1;
}

CGrid::~CGrid()
{
	delete m_Position;
	mptr_hexGrid = nullptr;
}

/*!
\todo add ability to load texture
*/

bool CGrid::Init()
{
	std::wstring wresourceFilenameVS, wresourceFilenameFS, wresourceFilenameTexture;
	std::string resourceFilenameVS, resourceFilenameFS, resourceFilenameTexture;

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

		return false;
	}

	if (!mptr_Render->createShaderProgram(
		&m_ColorProgramID,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return false;
	}

	CreateGrid();

	return true;
}


bool CGrid::Start()
{
	if (mptr_GameWindow->create("Hexagrid App"))
	{
		Init();

		// Set initial clear screen color
		mptr_Render->setClearScreenColor(0.25f, 0.0f, 0.75f);

		// Initialize window width/height in the renderer
		mptr_Render->setWindowWidth(mptr_GameWindow->getWidth());
		mptr_Render->setWindowHeight(mptr_GameWindow->getHeight());

		if (m_init)
		{
			mptr_Render->setWireframePolygonMode();

			// Enter main loop
			cout << "Entering Main loop" << endl;

			mptr_GameWindow->mainLoop(mptr_hexGrid);
		}
	}
	return true;
}

void CGrid::Render(){

	float Color[3] = { 0.5f,0.1f,0.5f };

	double totalDegreesRotatedRadians = 0.0;

	totalDegreesRotatedRadians = 0.2 * 3.1459 / 180.0;

	CVector3 pos2(1, 1, 1);

	MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, pos2);

	//MathHelper::Matrix4 modelMatrix2 = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, pos2);

	unsigned int noTexture = 0;

	mptr_Render->renderObject(
		&m_ColorProgramID,
		&m_VertexArrayObjectID,
		&noTexture,
		m_hexGridFaces,
		Color,
		&modelMatrix,
		COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
		false
	);
}

void CGrid::CreateGrid()
{
	// represents the individual hexagons 
	std::vector<CGridCell> Cells;

	for (int i = 0; i < m_CountHexagon; ++i)
	{
		Cells.emplace_back(CGridCell(1, -3, -2));
	}

	m_hexGridFaces = Cells.size() * 6;

	// without this 'if' the hexagon will look like a line 
	if (Cells.size() == 0)
	{
		Cells[0].CreateFlatTopHexagon();
		Cells[0].PrintPositions();
	}

	// needs i to be equal to 1 
	// position of the hexagons
	/*When creating hexagon we used the position
	of the last one's place to know where the next one
	goes */

	uint32_t CountToMoveUp = 0;
	bool IsGoingUp = false;

	for (int i = 1; i < m_CountHexagon; ++i)
	{
		if( CountToMoveUp > 0 && (CountToMoveUp % m_hexGridWidth == 0) )
		{
			Cells[i] = Cells[i - m_hexGridWidth];
			Cells[i].MoveUp();
			Cells[i].PrintPositions();
			IsGoingUp = false;
		}
		else if (i == 1)
		{
			Cells[0].SetZ(-2);
			Cells[0].CreateFlatTopHexagon();
			Cells[0].PrintPositions();
			Cells[1] = Cells[0];
			Cells[1].MoveUpRight();
			Cells[1].PrintPositions();
			CountToMoveUp++;
		}
		else
		{
			// copy position
			Cells[i] = Cells[i - 1];

			if (IsGoingUp== false)
			{
				Cells[i].MoveDownRight();
				Cells[i].PrintPositions();
				IsGoingUp = true;
			}
			else
			{
				Cells[i].MoveUpRight();
				Cells[i].PrintPositions();
				IsGoingUp = false;
			}
		}
		CountToMoveUp++;
	}

	float *ptr_Vertices = nullptr;
	float *ptr_FakeNormals = (float*)calloc(m_hexGridFaces * 3, sizeof(float));
	float *ptr_FakeUVCoords = (float*)calloc(Cells.size() * 6 * 2, sizeof(float));

	// fake normal indices 
	unsigned short *ptr_FakeNormalIndices = new unsigned short[m_hexGridFaces * 3];

	void *ptr_ResetFakeNormalIndices = ptr_FakeNormalIndices;

	for (size_t i = 0; i < m_hexGridFaces; i++)
	{
		(*ptr_FakeNormalIndices) = i;
		ptr_FakeNormalIndices++;
		(*ptr_FakeNormalIndices) = i;
		ptr_FakeNormalIndices++;
		(*ptr_FakeNormalIndices) = i;
		ptr_FakeNormalIndices++;
	}

	// reset the pointer to initial position
	ptr_FakeNormalIndices = static_cast<unsigned short*>(ptr_ResetFakeNormalIndices);

	// get the vertices's
	GetVertices(Cells, ptr_Vertices);

	unsigned short *ptr_IndicesVerts = new unsigned short[Cells.size() * 7 * 3];
	// used to restart 
	void *ptr_ResetIndeces = ptr_IndicesVerts;

	// Calculate vertices's
	printf("Printing Indices's for hexGrid\n");
	for (int i = 0; i < Cells.size(); ++i)
	{

		for (int j = 0; j < 6; ++j)
		{
			if (j != 5)
			{
				(*ptr_IndicesVerts) = i * 7;
				std::cout << '<' << *ptr_IndicesVerts << ',';
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i * 7) + 1 + j;
				std::cout << *ptr_IndicesVerts << ",";
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i * 7) + 2 + j;
				std::cout << *ptr_IndicesVerts << ">\n ";
				ptr_IndicesVerts++;
			}
			else
			{
				(*ptr_IndicesVerts) = i * 7;
				std::cout << '<' << *ptr_IndicesVerts << ',';
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i * 7) + 6;
				std::cout << *ptr_IndicesVerts << ",";
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i * 7) + 1;
				std::cout << *ptr_IndicesVerts <<">\n";
				ptr_IndicesVerts++;
			}
		}
	}

	ptr_IndicesVerts = static_cast<unsigned short*>(ptr_ResetIndeces);

	bool Continue = mptr_Render->allocateGraphicsMemoryForObject(&m_ColorProgramID,
																															 &m_VertexArrayObjectID,
																															 ptr_Vertices, (Cells.size() * 7 *3 ) / 3,
																															 ptr_FakeNormals, (Cells.size() * 6),
																															 ptr_FakeUVCoords, (Cells.size() * 7),
																															 ptr_IndicesVerts, m_hexGridFaces,//cuantas caras hay 
																															 ptr_FakeNormalIndices, m_hexGridFaces * 3,
																															 ptr_IndicesVerts, (Cells.size() * 7  * 3) / 3);

	if (!Continue)
	{
		m_hexGridFaces = 0;

		if (m_VertexArrayObjectID > 0)
		{
			mptr_Render->freeGraphicsMemoryForObject(&m_VertexArrayObjectID);
			m_VertexArrayObjectID = 0;
		}
	}

	if (ptr_Vertices != nullptr)
	{
		delete[]ptr_Vertices;
	}
	if (ptr_FakeNormalIndices != nullptr)
		delete[]ptr_FakeNormalIndices;
	if (ptr_FakeNormals != nullptr)
		free(ptr_FakeNormals);
	if (ptr_FakeUVCoords != nullptr)
		free(ptr_FakeUVCoords);
	if (ptr_IndicesVerts != nullptr)
		delete[]ptr_IndicesVerts;

	ptr_ResetFakeNormalIndices = nullptr;
	ptr_ResetIndeces = nullptr;
}


void CGrid::GetVertices(std::vector<CGridCell>& Vec, float *& ptr_Out)
{
	ptr_Out = new float[Vec.size() * 7 * 3];

	for (size_t i = 0; i < Vec.size(); i++)
	{
		Vec[i].TrasnferVertices(ptr_Out, i);
	}

}
