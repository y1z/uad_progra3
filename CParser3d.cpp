#include "CParser3d.h"



CParser3d::CParser3d()
{
}


CParser3d::~CParser3d()
{
	// Raw Values 
	if (mptr_Vertrice != nullptr) { delete[]mptr_Vertrice; }
	if (mptr_UVCoords != nullptr) { delete[]mptr_UVCoords; }
	if (mptr_Normals != nullptr) { delete[]mptr_Normals; }
	// Indices 
	if (mptr_VertriceIndice != nullptr) { delete[]mptr_VertriceIndice; }
	if (mptr_UVIndice != nullptr) { delete[]mptr_UVIndice; }
	if (mptr_NormalIndice != nullptr) { delete[]mptr_NormalIndice; }
}

bool CParser3d::ParseFBXFile(const char * FilePath)
{
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	if (!CWideStringHelper::GetResourceFullPath(FilePath
			, wresourceFilenameTexture, resourceFilenameTexture))
	{
		std::cerr << "----------------ERROR : Invalid file Path--------------------";
		return false;
	}

	/*! ComparasionStr is use to search for tokens */
	string ComparasionStr;
	string ResultStr;
	/*! To know when we find the part of the file we need */
	bool isInformationRelevant = false;

	m_FileReader.open(resourceFilenameTexture.c_str());

	if (m_FileReader.is_open())
	{
		char temp = '\0';
		// this token we tell us wen we get to the other tokens 
		constexpr char *TokenObjects = "Objects:";

		constexpr char *TokenVertices = "Vertices:";
		constexpr char *TokenPolygonVertexIndex = "PolygonVertexIndex:";
		constexpr char *TokenEdges = "Edges:";
		constexpr char *TokenNormals = "Normals:";
		constexpr char *TokenUV = "UV:";
		constexpr char *TokenUVIndex = "UVIndex:";

		short *ptr_TempIndeceContainment = nullptr;

		/*! where the value are going to be stored */
		uint8_t LengthOfToken = 0;

		bool HaveUV = false;
		do
		{
			m_FileReader >> temp;
			if (!isInformationRelevant && temp == 'O')
			{
				LengthOfToken = strlen(TokenObjects);
				for (size_t i = 0; i < LengthOfToken; i++)
				{
					ComparasionStr += temp;
					m_FileReader >> temp;
				}
				if (ComparasionStr == TokenObjects)
				{
					isInformationRelevant = true;
				}
				// This else is for skipping the line
				// and get to the next one 
				else
				{
					std::getline(m_FileReader, ComparasionStr);
				}
			}
			else if (isInformationRelevant)
			{
				bool GoodToken = false;
				if (temp == 'V')
				{
					GoodToken = isTokenValid(m_FileReader, ComparasionStr, TokenVertices, temp);
					if (GoodToken)
					{
						GetValueFromStr(m_FileReader, mptr_Vertrice, m_numVertices);
					}
				}
				else if (temp == 'N')
				{
					GoodToken = isTokenValid(m_FileReader, ComparasionStr, TokenNormals, temp);
					if (GoodToken)
					{
						GetValueFromStr(m_FileReader, mptr_Normals, m_numNormals);
					}
				}
				else if (temp == 'P')
				{
					GoodToken = isTokenValid(m_FileReader, ComparasionStr, TokenPolygonVertexIndex, temp);
					if (GoodToken)
					{
						GetValueFromStr(m_FileReader, ptr_TempIndeceContainment, m_numVertices);
					}
				}
				else if (temp == 'U' && !HaveUV)
				{
					GoodToken = isTokenValid(m_FileReader, ComparasionStr, TokenUV, temp);
					if (GoodToken)
					{
						GetValueFromStr(m_FileReader, mptr_UVCoords, m_numUVs);
						HaveUV = true;
					}
				}
				else if (HaveUV && temp == 'U')
				{
					GoodToken = isTokenValid(m_FileReader, ComparasionStr, TokenUVIndex, temp);
					if (GoodToken)
					{
						GetValueFromStr(m_FileReader, mptr_UVIndice, m_numUVIndex);
					}
				}
				else
				{
					getline(m_FileReader, ComparasionStr);
				}

			}
			ComparasionStr.clear();
		} while (!m_FileReader.eof());

		mptr_VertriceIndice = new unsigned short[m_numVertices];

		// convert indice's to something useful 
		for (size_t i = 0; i < m_numVertices; i++)
		{
			if (ptr_TempIndeceContainment[i] < 0)
			{
				ptr_TempIndeceContainment[i] = (abs(ptr_TempIndeceContainment[i])) - 1;
			}
			mptr_VertriceIndice[i] = ptr_TempIndeceContainment[i];
		}

		if (ptr_TempIndeceContainment != nullptr) { delete[]ptr_TempIndeceContainment; }

	}
	else
	{
		return false;
	}

	return true;
}

FBXContainer CParser3d::GetModelData(FBXContainer &Result)
{	

	Result.m_numFaces = this->m_numFaces;
	Result.m_numNormals = this->m_numNormals;
	Result.m_numVertices = this->m_numVertices;
	Result.m_numUVIndex = this->m_numUVIndex;
	Result.m_numUVs = this->m_numUVs;
	Result.m_numUVIndex = this->m_numUVIndex;

	Result.mptr_Vertices = this->mptr_Vertrice;
	Result.mptr_Normals = this->mptr_Normals;
	Result.mptr_UVCoords = this->mptr_UVCoords;

	Result.mptr_NormalIndice = this->mptr_NormalIndice;
	Result.mptr_UVIndice = this->mptr_UVIndice;
	Result.mptr_VertriceIndice = this->mptr_VertriceIndice;
	
	return Result;
}

bool CParser3d::isTokenValid(ifstream & inFile, string & Str, const char * Token, char CurrentChr)
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

void CParser3d::GetValueFromStr(ifstream & inFile, float *& ptr_Value, uint32_t & Count)
{
	char temp = '0';
	string NumReader;
	// find out how many elements to parse
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// Move past the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_Value = new float[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_Value[index] = stof(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_Value[index] = stof(NumReader);
		}
	}

}

void CParser3d::GetValueFromStr(ifstream & inFile, unsigned short *& ptr_Value, uint32_t & Count)
{
	char temp = '0';
	string NumReader;
	// find out how many elements to parse
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// Move past the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_Value = new unsigned short[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_Value[index] = stoi(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_Value[index] = stoi(NumReader);
		}
	}

}

void CParser3d::GetValueFromStr(ifstream & inFile, short *& ptr_Value, uint32_t & Count)
{
	char temp = '0';
	string NumReader;
	// find out how many elements to parse
	while (temp != '{')
	{
		inFile >> temp;
		NumReader += temp;
	}
	// gets rid of the char '{'
	NumReader.resize(NumReader.size() - 1);
	Count = stoi(NumReader);
	NumReader.clear();
	// Move past the chars 'a' and ':' 
	inFile >> temp;
	inFile >> temp;

	ptr_Value = new short[Count];
	int index = 0;

	while (temp != '}')
	{
		inFile >> temp;
		NumReader += temp;
		if (temp == ',')
		{
			ptr_Value[index] = stoi(NumReader);
			index++;
			NumReader.clear();
		}
		else if (temp == '}')
		{
			// gets rid of the char '{'
			NumReader.resize(NumReader.size() - 1);
			ptr_Value[index] = stoi(NumReader);
		}
	}

}
