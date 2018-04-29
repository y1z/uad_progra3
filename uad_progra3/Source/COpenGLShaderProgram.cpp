#include "../stdafx.h"

#include <map>
#include <string>
using namespace std;

#include "../Include/COpenGLShaderProgram.h"

/*
*/
COpenGLShaderProgram::COpenGLShaderProgram() :
	m_shaderProgramID(0)
{
	// Initialize known uniform parameters
	m_uniformLocations.insert(std::make_pair(UNIFORM_MODEL_MATRIX, 0));
	m_uniformLocations.insert(std::make_pair(UNIFORM_VIEW_MATRIX, 0));
	m_uniformLocations.insert(std::make_pair(UNIFORM_PROJECTION_MATRIX, 0));
	m_uniformLocations.insert(std::make_pair(UNIFORM_COLOR, 0));
	m_uniformLocations.insert(std::make_pair(UNIFORM_TEXTURE_SAMPLER, 0));

	// Initialize known attrib parameters
	m_attributeLocations.insert(std::make_pair(ATTRIBUTE_POSITION, 0));
	m_attributeLocations.insert(std::make_pair(ATTRIBUTE_NORMAL, 0));
	m_attributeLocations.insert(std::make_pair(ATTRIBUTE_UV, 0));
	m_attributeLocations.insert(std::make_pair(ATTRIBUTE_COLOR, 0));
}

/*
*/
COpenGLShaderProgram::~COpenGLShaderProgram()
{
	m_uniformLocations.clear();
	m_attributeLocations.clear();
}

/*
*/
int COpenGLShaderProgram::getUniformLocation(std::string uniformName) const
{
	auto it = m_uniformLocations.find(uniformName);

	if (it != m_uniformLocations.end())
	{
		return it->second;
	}

	return -1;
}

/*
*/
void COpenGLShaderProgram::setUniformLocation(std::string uniformName, int id)
{
	auto it = m_uniformLocations.find(uniformName);

	if (it != m_uniformLocations.end())
	{
		it->second = id;
	}
	else
	{
		m_uniformLocations.insert(std::make_pair(uniformName, id));
	}
}

/*
*/
int COpenGLShaderProgram::getAttributeLocation(std::string attributeName) const
{
	auto it = m_attributeLocations.find(attributeName);

	if (it != m_attributeLocations.end())
	{
		return it->second;
	}

	return -1;
}

/*
*/
void COpenGLShaderProgram::setAttributeLocation(std::string attribName, int id)
{
	auto it = m_attributeLocations.find(attribName);

	if (it != m_attributeLocations.end())
	{
		it->second = id;
	}
	else
	{
		m_attributeLocations.insert(std::make_pair(attribName, id));
	}
}