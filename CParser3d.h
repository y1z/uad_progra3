#pragma once
#include <iostream>
#include <fstream>
#include <cinttypes>
#include <filesystem>// requires c++ 17 
#include "Include/CWideStringHelper.h" // just for the project 
#include "Include/C3DModel.h"
#include "SDataContainter.h"
/*!
\brief This class reads a 3d file format convert to necessary
information in order to render a 3d model
\version 0.20
\author Yhaliff Said Barraza
*/
class CParser3d
{
public:
	CParser3d();
	~CParser3d();
private: // variables 
	/*! General attributes of 3d models*/
	uint32_t m_numFaces = 0;//!< how many face a model has 
	uint32_t m_numNormals = 0;//!< how many normal it has 
	uint32_t m_numVertices = 0;//!< how many vertices it has 
	uint32_t m_numUVs = 0;//!<how many UVs it has 
	uint32_t m_numUVIndex = 0;//!< how many indices it has 

	//! this is used to read the file 
	ifstream m_FileReader;

	/*Variables that contain the Raws values */

	float* mptr_Vertrice = nullptr;//!< will contain the values of the vertices 
	float* mptr_UVCoords = nullptr;//!< will contain themptr_Vertrice coordinates for the UVs 
	float* mptr_Normals = nullptr;//!< will contain the normals 

	/*Variables that contain the Order of the values */
	unsigned short* mptr_VertriceIndice = nullptr;//!<Contains the order of the vertices 
	unsigned short* mptr_UVIndice = nullptr;//! <Contains the order of the UVs
	unsigned short* mptr_NormalIndice = nullptr;//! <Contains the order of the Normals


public:// functions 
	bool ParseFBXFile(const char *FilePath);
	FBXContainer GetModelData(FBXContainer &Result);

private:

	/*!
\fn isTokenValid checks input from a file and then checks if the token is there.
\param inFile is where we receive input from .
\param Str use for comparison between itself and the Token .
\param Token is the token we are looking for .
\param CurrentChr is the char that is actively reading the file .
*/
	bool isTokenValid(ifstream &inFile, string &Str, const char *Token, char CurrentChr);


	/*!
	\param inFile is the current file we are reading
	\param [out] ptr_Value is a pointer that will receive numeric values from file (can be int or float )
	\param [out] Count to know how many values we are going to receive
	*/
	void GetValueFromStr(ifstream &inFile, float *&ptr_Value, uint32_t &Count);
	void GetValueFromStr(ifstream &inFile, unsigned short *&ptr_Value, uint32_t &Count);
	void GetValueFromStr(ifstream &inFile, short *&ptr_Value, uint32_t &Count);
};

