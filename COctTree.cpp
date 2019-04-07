#include "COctTree.h"

COctTree::COctTree()
{
}

COctTree::~COctTree()
{
	if (RootNode != nullptr) { delete RootNode; }
}

/*!uses data from a model to know how to make
the root node */
void COctTree::Init(FBXContainer &GeometryData)
{
	void *ResetPointer =	GeometryData.mptr_Vertices;

	float InitZ = std::numeric_limits<float>::lowest();
	float InitY = std::numeric_limits<float>::lowest();
	float InitX = std::numeric_limits<float>::lowest();

	for (int i = 0; i < GeometryData.m_numVertices / 3; ++i)
	{
		if (*GeometryData.mptr_Vertices > InitX) 
		{ InitX = *GeometryData.mptr_Vertices; }

		GeometryData.mptr_Vertices++;

		if (*GeometryData.mptr_Vertices > InitY) 
		{ InitY = *GeometryData.mptr_Vertices; }

		GeometryData.mptr_Vertices++;

		if (*GeometryData.mptr_Vertices > InitZ)
		{	InitZ = *GeometryData.mptr_Vertices; }

		GeometryData.mptr_Vertices++;
	}

	GeometryData.mptr_Vertices = (float*)ResetPointer;

	if(InitX < 0|| InitY < 0)
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

void COctTree::PrintContent()
{
	RootNode->InitPrintContent();
}

DWORD __stdcall COctTree::ThreadFunction(LPVOID param)
{
	COctTree* CurrentOctree = (COctTree*)param;
	CurrentOctree->Init(*CurrentOctree->Model);
	CurrentOctree->InsertValues(*CurrentOctree->Model);
	CurrentOctree->PrintContent();

	return 0;
}
