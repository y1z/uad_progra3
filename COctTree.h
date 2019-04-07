#pragma once
#include"stdafx.h"
#include "MacrolesWindow.h"
#include "SDataContainter.h"
#include "COctNode.h"

/*!
\class COctTree "COctTree.h"
\brief this class creates an OctTree
through the use of COctNode in other words
it's the boss of the nodes that do the actual work */

class COctTree
{
public:// constructor
	COctTree();
	~COctTree();
private:// variables 
	COctNode *RootNode = nullptr;

public:
	bool isDynamic = false;
	FBXContainer *Model = nullptr;
private:
	short * mptr_Indeces = nullptr;
public:// functions 
	void Init(FBXContainer &GeometryData);
	void InsertValues(FBXContainer &GeometryData);
	void PrintContent();
	void GetGeomety(FBXContainer &GeometryData);

	static DWORD WINAPI ThreadFunction(LPVOID param);
};