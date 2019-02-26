#include "CGrid.h"

CGrid::CGrid(COpenGLRenderer * ptr_renderer, CGameWindow * ptr_GameWindow, uint32_t HexCount, CAppHexgrid &HexGrid)
	:m_Render(ptr_renderer),m_GameWindow(ptr_GameWindow),m_CountHexagon(HexCount)
{
	m_hexGrid = &HexGrid;
}

CGrid::~CGrid()
{}
/*!
\todo finish inti and render 
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

	if (!m_Render->createShaderProgram(
		&m_ColorProgramID,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()))
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return false;
	}

	CreatGrid();


	return true;
}

/*if (m_GameWindow->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
	{
		Init();

		// Set initial clear screen color
		m_Render->setClearScreenColor(0.25f, 0.0f, 0.75f);

		// Initialize window width/height in the renderer
		m_Render->setWindowWidth(m_GameWindow->getWidth());
		m_Render->setWindowHeight(m_GameWindow->getHeight());

		if (m_init)
		{
			m_Render->setWireframePolygonMode();

			// Enter main loop
			cout << "Entering Main loop" << endl;
			m_GameWindow->mainLoop(this);
		}
	}
	return true;*/

bool CGrid::Start()
{
	if (m_GameWindow->create(CAPP_PROGRA3_GEOMETRIC_WINDOW_TITLE))
	{
		Init();

		// Set initial clear screen color
		m_Render->setClearScreenColor(0.25f, 0.0f, 0.75f);

		// Initialize window width/height in the renderer
		m_Render->setWindowWidth(m_GameWindow->getWidth());
		m_Render->setWindowHeight(m_GameWindow->getHeight());

		if (m_init)
		{
			m_Render->setWireframePolygonMode();

			// Enter main loop
			cout << "Entering Main loop" << endl;
			m_GameWindow->mainLoop(m_hexGrid);
		}
	}
	return true;
}

void CGrid::Render(){
	// USE THIS 
	/*			// Render pyramid in the first position, using the color shader
			getOpenGLRenderer()->renderObject(
				&m_colorModelShaderId,
				&m_pyramidVertexArrayObject,
				&noTexture,
				m_numFacesPyramid,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);*/
}

void CGrid::CreatGrid()
{
	std::vector< CGridCell> Cells;
	for(int i =0; i < m_CountHexagon; ++i)
	{
		Cells.emplace_back(CGridCell(1, 0, 0));
	} 

	m_hexGridFaces = Cells.size() * 6;

	// needs i to be equal to 1 
	// position of the hexagons
	for (int i = 1; i < m_CountHexagon; ++i)
	{
		Cells[i] = Cells[i - 1];
		
		if (i % 2 == 0)
		{
			Cells[i].MoveDownRight();
		}
		else
		{
			Cells[i].MoveUpRight();
		}	

	}

	float *ptr_Vertices = nullptr;
	float *ptr_FakeNormals =(float*)calloc(m_hexGridFaces, sizeof(float));
	float *ptr_FakeUVCoords = (float*)calloc(Cells.size() * 6 * 2, sizeof(float));
	GetVertices(Cells, ptr_Vertices);

	unsigned short *ptr_IndicesVerts = new unsigned short[Cells.size() * 6];
	for (int i = 0; i < Cells.size() -1; ++i)
	{
		for(int j = 0; j < 6; ++j)
		{
			if (j != 5)
			{
				(*ptr_IndicesVerts) = i * 18;
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i + 1 + j) * 18;
				ptr_IndicesVerts++;
				(*ptr_IndicesVerts) = (i + 2 + j) * 18;
				ptr_IndicesVerts++;
			}
			else{
				(*ptr_IndicesVerts) = i * 18;
				ptr_IndicesVerts++;

			}

		}
	}

//m_Render->allocateGraphicsMemoryForObject(m_ColorProgramID, m_VertexArrayObjectID)


	if (ptr_Vertices != nullptr)
		delete[]ptr_Vertices;
	if (ptr_FakeNormals != nullptr)
		free(ptr_FakeNormals);
	if (ptr_FakeUVCoords != nullptr)
		free(ptr_FakeUVCoords);
	if (ptr_IndicesVerts != nullptr)
		delete[]ptr_IndicesVerts;
}

void CGrid::GetVertices(std::vector<CGridCell>& Vec, float *& ptr_Out)
{
	ptr_Out = new float[Vec.size() * 7 * 3];
	
	for (size_t i = 0; i < Vec.size() - 1; i++)
	{
	 Vec[i].TransferValue(ptr_Out,i);
	}

}
