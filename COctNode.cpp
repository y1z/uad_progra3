#include "COctNode.h"
/**/
COctNode::COctNode()
{
	for (auto &Node : mptr_OtherNodes)
	{
		Node = nullptr;
	}

	CalculateVolume(m_Size, 0.0f);
}
/**/
COctNode::COctNode(float size, float ZPos)
	:m_Size(size)
{
	for (auto &Node : mptr_OtherNodes)
	{
		Node = nullptr;
	}
	CalculateVolume(m_Size, ZPos);
}

COctNode::COctNode(CVector3 UsedVector[8], NodeSelection NewNode, float size)
	:m_Size(size), m_SelectedNode(NewNode)
{
	for (auto &Node : mptr_OtherNodes)
	{
		Node = nullptr;
	}
	CalculateVolume(m_Size / 2, UsedVector[0].getZ());
}

/**/
COctNode::~COctNode()
{
	for (auto& Node : mptr_OtherNodes)
	{
		if (Node != nullptr)
		{
			delete Node;
		}
	}

}

/**/
void COctNode::CalculateVolume(float Size, float InitPosZ)
{
	for (auto &vectors : m_CubeCorners)
	{
		vectors.X = Size;
		vectors.Y = Size;
		vectors.Z = InitPosZ;
	}

	// equivalent to 45 degrees 
	float AngleForCube = 0.78539816f;

	for (char i = 0; i < 4; ++i)
	{
		m_CubeCorners[i].X = m_CubeCorners[i].X * cosf(AngleForCube);
		m_CubeCorners[i].Y = m_CubeCorners[i].Y * sinf(AngleForCube);
		// 1.570796f is equivalent to 90 degrees
		AngleForCube += 1.570796f;
	}

	float ZLength = CalculateZLength();

	for (char i = 4; i < 8; ++i)
	{
		m_CubeCorners[i].X = m_CubeCorners[i - 4].getX();
		m_CubeCorners[i].Y = m_CubeCorners[i - 4].getY();
		m_CubeCorners[i].Z = m_CubeCorners[i - 4].getZ() - ZLength;
	}

}

/* Overload used to create Smaller Partition*/
void COctNode::CalculateSmallerVolume(float Size, CVector3 OldPositions[8])
{

	for (int i = 0; i < 8; ++i)
	{
		OldPositions[i].X = Size;
		OldPositions[i].Y = Size;
	}

	// equivalent to 45 degrees 
	float AngleForCube = 0.78539816f;

	for (char i = 0; i < 4; ++i)
	{
		OldPositions[i].X = OldPositions[i].X * cosf(AngleForCube);
		OldPositions[i].Y = OldPositions[i].Y * sinf(AngleForCube);
		// 1.570796f is equivalent to 90 degrees
		AngleForCube += 1.570796f;
	}

	float ZLength = CalculateZLength() / 2;

	for (char i = 4; i < 8; ++i)
	{
		OldPositions[i].X = OldPositions[i - 4].getX();
		OldPositions[i].Y = OldPositions[i - 4].getY();
		OldPositions[i].Z = OldPositions[i - 4].getZ() - ZLength;
	}
}

/*! Calculates what the Distance of Z should be
using 2 already defined points */
float COctNode::CalculateZLength()
{
	float Points[2] = { m_CubeCorners[0].X,m_CubeCorners[1].X };
	//! this is the distance formula https://www.purplemath.com/modules/distform.htm
	float Result = sqrtf((pow(Points[1] - Points[0], 2)));

	return Result;
}

/**/
void COctNode::InitNewNode(float point[3])
{
	CVector3 NewPositions[8];
	// save all Previous Position for later 
	for (int i = 0; i < 8; ++i)
	{
		NewPositions[i] = m_CubeCorners[i];
	}

	// Create Volume for Smaller partition 
	CalculateSmallerVolume(m_Size / 2, NewPositions);

	/**/
	FindClosetPartition(point, NewPositions);
}
/*! Finds the closest node to the point*/
void COctNode::FindClosetPartition(float point[3], CVector3 NewPositions[8])
{
	// setting to the highest value possible for later finding the 
	// Closest node
	float Distance = std::numeric_limits<float>::max();

	char ClosestNodeIndex = 0;
	// distance formula 
	for (int i = 0; i < 8; ++i)
	{
		float DeltaX = (m_CubeCorners[i].getX() - point[0]);
		float DeltaY = (m_CubeCorners[i].getY() - point[1]);
		float DeltaZ = (m_CubeCorners[i].getZ() - point[2]);

		DeltaX = DeltaX * DeltaX;
		DeltaY = DeltaY * DeltaY;
		DeltaZ = DeltaZ * DeltaZ;

		float Test = sqrt(DeltaX + DeltaY + DeltaZ);
		// taking the closest node 
		if (Test < Distance)
		{
			Distance = Test;
			ClosestNodeIndex = i;
		}
	}

	NodeSelection Node = static_cast<NodeSelection>(ClosestNodeIndex);

	MoveToPosition(Node, NewPositions, point);
}

/*! This is to determine which point is connected with
which partition*/
void COctNode::FindNodePartition(float point[3], CVector3 PosibleNewPositions[8])
{
	float Distance = std::numeric_limits<float>::max();

	char ClosestNodeIndex = 0;

	for (int i = 0; i < 8; ++i)
	{
		float DeltaX = (m_CubeCorners[i].getX() - point[0]);
		float DeltaY = (m_CubeCorners[i].getY() - point[1]);
		float DeltaZ = (m_CubeCorners[i].getZ() - point[2]);

		DeltaX = DeltaX * DeltaX;
		DeltaY = DeltaY * DeltaY;
		DeltaZ = DeltaZ * DeltaZ;

		float Test = sqrt(DeltaX + DeltaY + DeltaZ);
		// taking the closest node 
		if (Test < Distance)
		{
			Distance = Test;
			ClosestNodeIndex = i;
		}
	}
	/*!used to know which node to go to */
	NodeSelection Node = static_cast<NodeSelection>(ClosestNodeIndex);

	MoveToPosition(Node, PosibleNewPositions, point);
}

void COctNode::InitPrintContent()
{
	std::string DepthTracker;
	DepthTracker.resize(0);
	DepthTracker.reserve(20);
	PrintContent(DepthTracker);
}

/*
\param Geometry [out] is used to contain all the vertices 
\param Depth [in] this value is used to know how much of the Octree you what to use 
*/
void COctNode::GetGeometry(std::deque<CVector3>& Geometry, int Depth)
{
	if (Depth <= 8)
	{
		for (int i = 0; i < Depth; ++i)
		{
			if (mptr_OtherNodes[i] != nullptr && 
					mptr_OtherNodes[i]->m_PointList.size() > 2)
			{
				for (auto &Vec : mptr_OtherNodes[i]->m_PointList)
				{
					Geometry.push_back(Vec);
				}
			}
		}

	}

}

/*! This prints a partition and the amount
of triangles if it has any.*/
void COctNode::PrintContent(std::string &DepthTracker)
{
	// To know how deep we are in the tree 
	char NodeDepth = (int)m_SelectedNode + '0' + 1;

	DepthTracker.push_back(NodeDepth);
	DepthTracker.push_back('.');

	for (int i = 0; i < 8; ++i)
	{
		if (mptr_OtherNodes[i] != nullptr
				&& mptr_OtherNodes[i]->m_PointList.size() >= 1)
		{
			// find out which node we are in
			NodeDepth = (int)mptr_OtherNodes[i]->m_SelectedNode + '0' + 1;

			DepthTracker.push_back(NodeDepth);
			DepthTracker.push_back('.');

			if (mptr_OtherNodes[i]->m_PointList.size() > 2)
			{
				uint32_t TotalTriangles = mptr_OtherNodes[i]->m_PointList.size() / 3;
				printf("%s = %d Triangulos\n", DepthTracker.c_str(), TotalTriangles);
			}
			else
			{
				printf("%s Has less than 3 points\n", DepthTracker.c_str());
			}
		}
		else
		{
			NodeDepth = (int)'0' + i + 1;

			DepthTracker.push_back(NodeDepth);
			DepthTracker.push_back('.');
			printf("%s  = '' \n", DepthTracker.c_str());
		}
		DepthTracker.resize(DepthTracker.size() - 2);
	}

	for (int i = 0; i < 8; ++i)
	{
		// to get Recursion 
		if (mptr_OtherNodes[i] != nullptr)
		{
			mptr_OtherNodes[i]->PrintContent(DepthTracker);
			DepthTracker.resize(DepthTracker.size() - 2);
		}
		else
		{
			char NodeDepth = (int) '0' + i + 1;
			DepthTracker.push_back(NodeDepth);
			DepthTracker.push_back('.');
			printf("%s = <Vacio>\n", DepthTracker.c_str());
			DepthTracker.resize(DepthTracker.size() - 2);
		}
	}

}


/*!Get The Distance For Each Axis
\param Origin [in] where the origin is in 3d space
\param Endpoint [in] where the endpoint is in 3d space
\param DistancesPerAxis [out] will be used to store the results */
void COctNode::CalculateDistaceInEveryAxis(CVector3 Origin, CVector3 Endpoint, float DistancesPerAxis[3])
{
	DistancesPerAxis[0] = sqrtf((pow(Endpoint.X - Origin.X, 2)));
	DistancesPerAxis[1] = sqrtf((pow(Endpoint.Y - Origin.Y, 2)));
	DistancesPerAxis[2] = sqrtf((pow(Endpoint.Z - Origin.Z, 2)));
}

/*!Finds where to move the points consisting of the new partition*/
void COctNode::MoveToPosition(NodeSelection WhereToMove, CVector3 NewPositons[8], float point[3])
{
	float DistancesPerAxis[3] = { 0.f,0.f,0.f };
	bool isAlreadyMade = false;

	if (mptr_OtherNodes[WhereToMove] != nullptr)
		isAlreadyMade = true;

	/*!Depending on the node some values may need to be added
		 or subtracted */
	switch (WhereToMove)
	{
		case COctNode::TopRightFront:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X += DistancesPerAxis[0];
				NewPositons[i].Y += DistancesPerAxis[1];
				NewPositons[i].Z += DistancesPerAxis[2];
			}
			break;
		case COctNode::TopLeftFront:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X -= DistancesPerAxis[0];
				NewPositons[i].Y += DistancesPerAxis[1];
				NewPositons[i].Z += DistancesPerAxis[2];
			}
			break;
		case COctNode::BottomLeftFront:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X -= DistancesPerAxis[0];
				NewPositons[i].Y -= DistancesPerAxis[1];
				NewPositons[i].Z += DistancesPerAxis[2];
			}
			break;
		case COctNode::BottomRightFront:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X += DistancesPerAxis[0];
				NewPositons[i].Y -= DistancesPerAxis[1];
				NewPositons[i].Z += DistancesPerAxis[2];
			}
			break;
		case COctNode::TopRightBack:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X += DistancesPerAxis[0];
				NewPositons[i].Y += DistancesPerAxis[1];
				NewPositons[i].Z -= DistancesPerAxis[2];
			}
			break;
		case COctNode::TopLeftBack:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X -= DistancesPerAxis[0];
				NewPositons[i].Y += DistancesPerAxis[1];
				NewPositons[i].Z -= DistancesPerAxis[2];
			}
			break;
		case COctNode::BottomLeftBack:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X -= DistancesPerAxis[0];
				NewPositons[i].Y -= DistancesPerAxis[1];
				NewPositons[i].Z -= DistancesPerAxis[2];
			}
			break;
		case COctNode::BottomRightBack:
			CalculateDistaceInEveryAxis(NewPositons[WhereToMove], m_CubeCorners[WhereToMove], DistancesPerAxis);
			for (int i = 0; i < 8; ++i)
			{
				NewPositons[i].X += DistancesPerAxis[0];
				NewPositons[i].Y -= DistancesPerAxis[1];
				NewPositons[i].Z -= DistancesPerAxis[2];
			}
			break;
	}

	if (this->m_NodeDepth <= this->m_NodeDepthLimit
			&& isAlreadyMade == false)
	{
		mptr_OtherNodes[WhereToMove] = new COctNode(NewPositons, WhereToMove, this->m_Size / 2);
		mptr_OtherNodes[WhereToMove]->m_NodeDepth = this->m_NodeDepth + 1;
		mptr_OtherNodes[WhereToMove]->InitNewNode(point);
	}
	else if (isAlreadyMade == true
					 && this->m_NodeDepth <= this->m_NodeDepthLimit)
	{
		mptr_OtherNodes[WhereToMove]->InitNewNode(point);
	}
	else
	{
		CVector3 Temp;
		Temp.setValues(point);
		m_PointList.emplace_back(Temp);
	}

	//used to debug 
	/*
	printf("\n\n\nHere are the New Positions \n------------------\n\n");
	for (int i = 0; i < 8; ++i)
	{
		NewPositons[i].PrintPositions();
	}*/
}


