#pragma once
#include <iostream>
#include <string>
#include <deque>
#include "MacrolesWindow.h"
#include <cmath>
#include <numeric>
#include <climits>
#include "Include/CVector3.h"
#include "Include/Globals.h"
/*!
\class COctNode COctNode.h
\brief this determines the volume and Position of the node for the
OctTree
*/

class COctNode
{
public:// variables
	COctNode *mptr_OtherNodes[8];
private:
	/*!will be used to determine the volume of
	the current node Partition*/
	CVector3 m_CubeCorners[8];

	/*!use to determine the size of the cube  */
	float m_Size = 1.0f;

	/*!used to know how many nodes deep we are in theOctTree*/
	uint8_t m_NodeDepth = 0;

	/*! used to know when to stop going deeper*/
	uint8_t m_NodeDepthLimit = 4;

	//! used to determine which node to select 
	enum NodeSelection : char
	{
		NONE = -1,
		// first half of cube 
		TopRightFront = 0,
		TopLeftFront = 1,
		BottomLeftFront = 2,
		BottomRightFront = 3,
		// second half of cube 
		TopRightBack = 4,
		TopLeftBack = 5,
		BottomLeftBack = 6,
		BottomRightBack = 7,
	};

	NodeSelection m_SelectedNode = NodeSelection::TopRightFront;

	/*! To know which points are in the node */
	std::deque<CVector3> m_PointList;

public:// functions  

	//void Push(CVector3 UsedVector[8], NodeSelection NewNode);

	void InitNewNode(float point[3]);
	void FindClosetPartition(float point[3], CVector3 PreviousPositions[8]);
	void FindNodePartition(float point[3], CVector3 PosibleNewPositions[8]);


	void InitPrintContent();

	void GetGeometry(std::deque<CVector3> &Geometry, int Depth);
private:
	void PrintContent(std::string &DepthTracker);

	void CalculateDistaceInEveryAxis(CVector3 Origin, CVector3 End, float Distances[3]);
	void CalculateVolume(float Size, float InitPosZ);
	void CalculateSmallerVolume(float Size, CVector3 PreiousPosition[8]);
	float CalculateZLength();

	void MoveToPosition(NodeSelection WhereToMove, CVector3 UsedVector[8],float Point[3]);

	

public:// constructor
	COctNode();
	COctNode(float size, float ZPos);
	COctNode(CVector3 UsedVector[8], NodeSelection NewNode,float size);
	~COctNode();
};

