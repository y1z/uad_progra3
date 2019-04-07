#include "COctTree.h"

COctTree::COctTree()
{
}

COctTree::~COctTree()
{
	if (RootNode != nullptr) { delete RootNode; }
	if (mptr_Indeces != nullptr) { delete mptr_Indeces; }

}

/*!uses data from a model to know how to make
the root node */
void COctTree::Init(FBXContainer &GeometryData)
{
	void *ResetPointer = GeometryData.mptr_Vertices;

	// trying to get the biggest value so the octree 
	// completely covers the volume
	float InitZ = std::numeric_limits<float>::lowest();
	float InitY = std::numeric_limits<float>::lowest();
	float InitX = std::numeric_limits<float>::lowest();

	// get vertices from GeometryData
	for (int i = 0; i < GeometryData.m_numVertices / 3; ++i)
	{
		if (*GeometryData.mptr_Vertices > InitX)
		{
			InitX = *GeometryData.mptr_Vertices;
		}

		GeometryData.mptr_Vertices++;

		if (*GeometryData.mptr_Vertices > InitY)
		{
			InitY = *GeometryData.mptr_Vertices;
		}

		GeometryData.mptr_Vertices++;

		if (*GeometryData.mptr_Vertices > InitZ)
		{
			InitZ = *GeometryData.mptr_Vertices;
		}

		GeometryData.mptr_Vertices++;
	}

	GeometryData.mptr_Vertices = (float*)ResetPointer;

	// this is in case all the values are negative 
	if (InitX < 0 || InitY < 0)
	{
		for (int i = 0; i < GeometryData.m_numVertices / 3; ++i)
		{
			if (*GeometryData.mptr_Vertices < InitX)
			{
				InitX = *GeometryData.mptr_Vertices;
			}

			GeometryData.mptr_Vertices++;

			if (*GeometryData.mptr_Vertices < InitY)
			{
				InitY = *GeometryData.mptr_Vertices;
			}

			GeometryData.mptr_Vertices++;
		}

		InitX *= -1;
		InitY *= -1;
	}

	GeometryData.mptr_Vertices = (float*)ResetPointer;

	if (InitZ > InitX && InitZ > InitY)
	{
		RootNode = new COctNode(InitZ, InitZ);
	}

	else	if (InitX > InitY)
	{
		RootNode = new COctNode(InitX, InitZ);
	}
	else
	{
		RootNode = new COctNode(InitY, InitZ);
	}

}
/*! Takes Vertices From FBXContainer */
void COctTree::InsertValues(FBXContainer &GeometryData)
{
	float Values[3] = {};

	void *ResetPointer = GeometryData.mptr_Vertices;

	for (int i = 0; i < GeometryData.m_numVertices / 3; ++i)
	{
		Values[0] = *GeometryData.mptr_Vertices;
		GeometryData.mptr_Vertices++;
		Values[1] = *GeometryData.mptr_Vertices;
		GeometryData.mptr_Vertices++;
		Values[2] = *GeometryData.mptr_Vertices;
		GeometryData.mptr_Vertices++;

		RootNode->InitNewNode(Values);
	}

	GeometryData.mptr_Vertices = (float*)ResetPointer;
}
/*!Print all the triangles in the Octree */
void COctTree::PrintContent()
{
	RootNode->InitPrintContent();
}
// GetGeomety and generates the indices  
void COctTree::GetGeomety(FBXContainer & GeometryData)
{
	mptr_Indeces = new short[GeometryData.m_numVertices];
	std::deque<CVector3> VerticeHolder;

	void *ResetPointer = mptr_Indeces;
	RootNode->GetGeometry(VerticeHolder, 4);

	short IndeceCount = 0;
	for (auto Vec : VerticeHolder)
	{
		*mptr_Indeces = IndeceCount++;
		mptr_Indeces++;
		*mptr_Indeces = IndeceCount++;
		mptr_Indeces++;
		*mptr_Indeces = IndeceCount++;
		mptr_Indeces++;
	}

	mptr_Indeces = (short*)ResetPointer;
}

DWORD __stdcall COctTree::ThreadFunction(LPVOID param)
{
	COctTree* CurrentOctree = (COctTree*)param;
	CurrentOctree->Init(*CurrentOctree->Model);
	if (CurrentOctree->isDynamic == false)
	{
		CurrentOctree->InsertValues(*CurrentOctree->Model);
		CurrentOctree->PrintContent();
	}
	else
	{
		CurrentOctree->GetGeomety(*CurrentOctree->Model);
	}

	return 0;
}
