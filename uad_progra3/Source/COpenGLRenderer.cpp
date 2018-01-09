#include "../stdafx.h"

// include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/COpenGLRenderer.h"
#include "../Include/LoadShaders.h"
#include "../Include/MathHelper.h"

/*
*/
COpenGLRenderer::COpenGLRenderer():
	m_OpenGLError{ false },
	m_cameraDistance{ MIN_CAMERA_DISTANCE }
{
}

/*
*/
COpenGLRenderer::~COpenGLRenderer()
{
	// Delete test objects
	if (mTestshaderProgramID != 0)
	{
		glDeleteProgram(mTestshaderProgramID);
		mTestshaderProgramID = 0;
	}

	if (mVertexPositionBuffer != 0)
	{
		deleteBufferObject(&mVertexPositionBuffer);
		mVertexPositionBuffer = 0;
	}

	if (mVertexColorBuffer != 0)
	{
		deleteBufferObject(&mVertexColorBuffer);
		mVertexColorBuffer = 0;
	}

	if (mIndexBuffer != 0)
	{
		deleteBufferObject(&mIndexBuffer);
		mIndexBuffer = 0;
	}

	if (mVertexPositionArrayObjectID != 0)
	{
		deleteVertexArrayObject(&mVertexPositionArrayObjectID);
	}
}

/*
*/
bool COpenGLRenderer::createShaderProgram(unsigned int *shaderProgramId, const char *vertexShader, const char *fragmentShader)
{
	if (shaderProgramId != NULL)
	{
		ShaderInfo shaders[3];

		shaders[0].type = GL_VERTEX_SHADER;
		shaders[0].filename = vertexShader;

		shaders[1].type = GL_FRAGMENT_SHADER;
		shaders[1].filename = fragmentShader;

		shaders[2].type = GL_NONE;
		shaders[2].filename = NULL;

		*shaderProgramId = (unsigned int)LoadShaders(shaders);

		return (*shaderProgramId > 0);
	}

	return false;
}

/*
*/
bool COpenGLRenderer::deleteShaderProgram(unsigned int *shaderProgramId)
{
	if (shaderProgramId != NULL && *shaderProgramId > 0)
	{
		glDeleteProgram(*shaderProgramId);
		*shaderProgramId = 0;
		return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::useShaderProgram(unsigned int *shaderProgramId) const
{
	if (shaderProgramId != NULL && *shaderProgramId > 0)
	{
		glUseProgram((GLuint)*shaderProgramId);
		return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::createTextureObject(unsigned int *textureObjectId, unsigned char *textureData, int width, int height)
{
	if (textureObjectId != NULL && textureData != NULL)
	{
		// Create an OpenGL texture object
		glGenTextures(1, textureObjectId);

		// Activate texture unit #0
		glActiveTexture(GL_TEXTURE0);

		// Bind the new texture objectly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, *textureObjectId);

		// Pass the data to OpenGL
		// *NOTE: We're specifying GL_BGR because we're reading TGA files, which have the RED and BLUE channels switched.
		//        Ideally, we should pass a parameter to specify this ;)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);

		// Set mipmap filtering modes and generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("COpenGLRenderer::createTextureObject");

		return true;
	}

	return false;
}


/*
*/
GLuint COpenGLRenderer::generateVertexArrayObjectID() const
{
	GLuint vAO_ID = 0;
	glGenVertexArrays(1, &vAO_ID);
	glBindVertexArray(vAO_ID);

	return vAO_ID;
}

/*
*/
void COpenGLRenderer::deleteVertexArrayObject(GLuint *id)
{
	glDeleteVertexArrays(1, id);
}

/*
*/
GLuint COpenGLRenderer::generateBufferObjectID(GLfloat * const vertices, int numVertices) const
{
	GLuint vBO_ID;
	glCreateBuffers(1, &vBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vBO_ID);
	glNamedBufferStorage(vBO_ID, sizeof(GLfloat) * numVertices * 2, vertices, 0);

	return vBO_ID;
}

/*
*/
void COpenGLRenderer::deleteBufferObject(GLuint *id)
{
	glDeleteBuffers(1, id);
}

/*
 * Enables the given vertex shader attribute in the CURRENTLY BOUND buffer
*/
void COpenGLRenderer::setCurrentVertexAttribPointer(int vertexShaderAttribPos, int numComponents, int type) const
{
	glVertexAttribPointer(
		vertexShaderAttribPos,  // shader attribute location index
		numComponents,          // number of components (x, y, z)
		type,                   // 
		GL_FALSE,               // don't normalize
		0,                      // stride = 0
		BUFFER_OFFSET(0));      // pointer to the data, in the currently bound buffer

	glEnableVertexAttribArray(vertexShaderAttribPos);
}

/*
*/
bool COpenGLRenderer::freeGraphicsMemoryForObject(unsigned int *shaderProgramId, unsigned int *vertexArrayObjectID)
{
	bool deletedShaderProgram = false;
	bool deletedVertexArrayObjectId = false;

	deletedShaderProgram = deleteShaderProgram(shaderProgramId);

	if (vertexArrayObjectID != NULL && *vertexArrayObjectID > 0)
	{
		deleteVertexArrayObject((GLuint *)vertexArrayObjectID);
		*vertexArrayObjectID = 0;
		deletedVertexArrayObjectId = true;
	}

	return (deletedShaderProgram && deletedVertexArrayObjectId);
}

/*
*/
bool COpenGLRenderer::allocateGraphicsMemoryForObject(
	unsigned int *shaderProgramId,
	const char *vertexShader,
	const char *fragmentShader,
	unsigned int *vertexArrayObjectID,
	GLfloat *vertices, int numVertices,
	GLfloat *normals, int numNormals,
	GLfloat *UVcoords, int numUVCoords,
	unsigned short *indicesVertices, int numIndicesVert,
	unsigned short *indicesNormals, int numIndicesNormals,
	unsigned short *indicesUVCoords, int numIndicesUVCoords)
{
	if (shaderProgramId == NULL || !createShaderProgram(shaderProgramId, vertexShader, fragmentShader))
	{
		return false;
	}

	if (vertexArrayObjectID != NULL)
	{
		*vertexArrayObjectID = 0;
	}

	if (vertexArrayObjectID != NULL
		&& *shaderProgramId > 0
		&& vertices != NULL 
		&& normals != NULL
		&& UVcoords != NULL
		&& indicesVertices != NULL
		&& indicesNormals != NULL
		&& indicesUVCoords != NULL)
	{
		GLuint vertexPositionBuffer = 0;
		GLuint normalsPositionBuffer = 0;
		GLuint uvCoordsPositionBuffer = 0;
		GLuint indicesVertexBuffer = 0;

		if (!useShaderProgram(shaderProgramId))
			return false;

		// NOTE: We're assuming these attrib and uniforms are in the shader

		// Attributes change per-vertex
		GLint vertexPosAttribLocation = glGetAttribLocation(*shaderProgramId, "attPosition");
		GLint normalPosAttribLocation = glGetAttribLocation(*shaderProgramId, "attNormal");
		GLint uvPosAttribLocation     = glGetAttribLocation(*shaderProgramId, "attUV");

		// Check attribute locations are valid
		if (vertexPosAttribLocation < 0
			|| normalPosAttribLocation < 0
			|| uvPosAttribLocation < 0)
		{
			cout << "Unable to get shader program attribute locations" << endl;
			cout << "vertexPosAttribLocation: " << vertexPosAttribLocation << endl;
			cout << "normalPosAttribLocation: " << normalPosAttribLocation << endl;
			cout << "uvPosAttribLocation: " << uvPosAttribLocation << endl;
			glUseProgram(0);
			return false;
		}

		// Uniforms change per-object
		sh_ModelUniformLocation = glGetUniformLocation(*shaderProgramId, "uModelMatrix");
		sh_ViewUniformLocation  = glGetUniformLocation(*shaderProgramId, "uViewMatrix");
		sh_ProjUniformLocation  = glGetUniformLocation(*shaderProgramId, "uProjMatrix");
		sh_colorUniformLocation = glGetUniformLocation(*shaderProgramId, "uColor");

		if (sh_ModelUniformLocation < 0
			|| sh_ViewUniformLocation < 0
			|| sh_ProjUniformLocation < 0
			|| sh_colorUniformLocation < 0)
		{
			cout << "Unable to get shader program uniform locations" << endl;
			cout << "sh_ModelUniformLocation: " << sh_ModelUniformLocation << endl;
			cout << "sh_ViewUniformLocation: " << sh_ViewUniformLocation << endl;
			cout << "sh_ProjUniformLocation: " << sh_ProjUniformLocation << endl;
			cout << "sh_colorUniformLocation: " << sh_colorUniformLocation << endl;
			glUseProgram(0);
			return false;
		}

		// There's no supported way to render multi-indexed buffers in OpenGL, we need to generate a
		// final set of vertices and duplicate vertices for the ones that share more than one normal or UV coord
		int finalNumTriangles = 0;
		int numFaces = numIndicesVert;
		int finalNumVertices = numFaces * 3 * 3; // Number of faces * 3 vertex indices * 3 components (x,y,z)
		int finalNumNormals  = numFaces * 3 * 3; // Number of faces * 3 normal indices * 3 components (x,y,z)
		int finalNumUVCoords = numFaces * 3 * 2; // Number of faces * 3 UV indices * 2 components (x, y)

		GLfloat *finalVertices = new GLfloat[finalNumVertices];
		GLfloat *finalNormals = new GLfloat[finalNumNormals];
		GLfloat *finalUVCoords = new GLfloat[finalNumUVCoords];

		memset(finalVertices, 0, sizeof(GLfloat) * finalNumVertices);
		memset(finalNormals, 0, sizeof(GLfloat) * finalNumNormals);
		memset(finalUVCoords, 0, sizeof(GLfloat) * finalNumUVCoords);

		// Flatten the arrays in the final geometry arrays
		if (!generateRenderGeometry(
			vertices, numVertices,
			normals, numNormals,
			UVcoords, numUVCoords,
			numIndicesVert,
			indicesVertices,
			indicesNormals,
			indicesUVCoords,
			finalVertices, finalNormals, finalUVCoords, &finalNumTriangles))
		{
			if (finalVertices != NULL)
			{
				delete[] finalVertices;
				finalVertices = NULL;
			}
			if (finalNormals != NULL)
			{
				delete[] finalNormals;
				finalNormals = NULL;
			}
			if (finalUVCoords != NULL)
			{
				delete[] finalUVCoords;
				finalUVCoords = NULL;
			}

			return false;
		}

		// Commented out ***, not using indices, we have flattened arrays

		// Create and bind a vertex array object
		*vertexArrayObjectID = (unsigned int)generateVertexArrayObjectID();

		// Generate a buffer for the vertices and set its data
		glGenBuffers(1, &vertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		//*** glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumVertices, finalVertices, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(vertexPosAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vertexPosAttribLocation);

		// Generate a buffer for the normals and set its data
		glGenBuffers(1, &normalsPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalsPositionBuffer);
		//*** glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numNormals * 3, normals, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumNormals, finalNormals, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(normalPosAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(normalPosAttribLocation);
		
		// Generate a buffer for the UV coords and set its data
		glGenBuffers(1, &uvCoordsPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvCoordsPositionBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numUVCoords * 2, UVcoords, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * finalNumUVCoords, finalUVCoords, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glVertexAttribPointer(uvPosAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(uvPosAttribLocation);

		// Generate a buffer for the triangle indices and set its data
		
		//*** glGenBuffers(1, &indicesVertexBuffer);
		//*** glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBuffer);
		//*** glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndicesVert * 3, indicesVertices, GL_STATIC_DRAW);

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//*** glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete buffers
		deleteBufferObject(&vertexPositionBuffer);
		deleteBufferObject(&normalsPositionBuffer);
		deleteBufferObject(&uvCoordsPositionBuffer);
		//*** deleteBufferObject(&indicesVertexBuffer);

		glUseProgram(0);

		// Free memory allocated for final render geometry, it's stored in the graphics card now
		if (finalVertices != NULL)
		{
			delete[] finalVertices;
			finalVertices = NULL;
		}
		if (finalNormals != NULL)
		{
			delete[] finalNormals;
			finalNormals = NULL;
		}
		if (finalUVCoords != NULL)
		{
			delete[] finalUVCoords;
			finalUVCoords = NULL;
		}

		return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::generateRenderGeometry(
	GLfloat *vertices, int numVertices,
	GLfloat *normals, int numNormals,
	GLfloat *UVcoords, int numUVCoords,
	int numFaces,
	unsigned short *indicesVertices, 
	unsigned short *indicesNormals,
	unsigned short *indicesUVCoords,
	GLfloat *finalVertices,
	GLfloat *finalNormals,
	GLfloat *finalUVCoords,
	int *numTriangles)
{
	int currentVertex = 0;
	int currentNormal = 0;
	int currentUV = 0;

	unsigned short vIndices[3];
	unsigned short nIndices[3];
	unsigned short uIndices[3];

	if (numTriangles == NULL
		|| finalVertices == NULL
		|| finalNormals == NULL
		|| finalUVCoords == NULL
		|| indicesVertices == NULL
		|| indicesNormals == NULL
		|| indicesUVCoords == NULL
		|| vertices == NULL
		|| normals == NULL
		|| UVcoords == NULL
		)
	{
		cout << "COpenGLRenderer::generateRenderGeometry() : Invalid pointers" << endl;
		return false;
	}

	*numTriangles = numFaces;

	// Iterate over each face
	for (int i = 0; i < (numFaces * 3); i+=3)
	{
		// Each face has 3 vertex indices
		// Each vertex has 3 components: x, y, z

		// Vertex indices for this face
		vIndices[0] = indicesVertices[i];   // Vertex index 1 
		vIndices[1] = indicesVertices[i+1]; // Vertex index 2
		vIndices[2] = indicesVertices[i+2]; // Vertex index 3

		// Normal indices for this face
		nIndices[0] = indicesNormals[i];    // Normal index 1
		nIndices[1] = indicesNormals[i+1];  // Normal index 2
		nIndices[2] = indicesNormals[i+2];  // Normal index 3

		// UVCoord indices for this face
		uIndices[0] = indicesUVCoords[i];   // UV coord index 1
		uIndices[1] = indicesUVCoords[i+1]; // UV coord index 2
 		uIndices[2] = indicesUVCoords[i+2]; // UV coord index 3

		if (((vIndices[0] * 3) + 2) >= (numVertices * 3)
			|| ((vIndices[1] * 3) + 2) >= (numVertices * 3)
			|| ((vIndices[2] * 3) + 2) >= (numVertices * 3))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid vertex indices" << endl;
			return false;
		}

		if (((nIndices[0] * 3) + 2) >= (numNormals * 3)
			|| ((nIndices[1] * 3) + 2) >= (numNormals * 3)
			|| ((nIndices[2] * 3) + 2) >= (numNormals * 3))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid normal indices" << endl;
			return false;
		}

		if (((uIndices[0] * 2) + 1) >= (numUVCoords * 2)
			|| ((uIndices[1] * 2) + 1) >= (numUVCoords * 2)
			|| ((uIndices[2] * 2) + 1) >= (numUVCoords * 2))
		{
			cout << "COpenGLRenderer::generateRenderGeometry() : Invalid UV coord indices" << endl;
			return false;
		}

		// For each vertex, copy its 3 components, finalVertices is an array of floats

		finalVertices[currentVertex + 0] = vertices[(vIndices[0] * 3) + 0]; // Vertex1 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[0] * 3) + 1]; // Vertex1 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[0] * 3) + 2]; // Vertex1 Z
		currentVertex += 3;
		finalVertices[currentVertex + 0] = vertices[(vIndices[1] * 3) + 0]; // Vertex2 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[1] * 3) + 1]; // Vertex2 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[1] * 3) + 2]; // Vertex2 Z
		currentVertex += 3;
		finalVertices[currentVertex + 0] = vertices[(vIndices[2] * 3) + 0]; // Vertex3 X
		finalVertices[currentVertex + 1] = vertices[(vIndices[2] * 3) + 1]; // Vertex3 Y
		finalVertices[currentVertex + 2] = vertices[(vIndices[2] * 3) + 2]; // Vertex3 Z
		currentVertex += 3;

		finalNormals[currentNormal + 0] = normals[(nIndices[0] * 3) + 0]; // Normal 1 X
		finalNormals[currentNormal + 1] = normals[(nIndices[0] * 3) + 1]; // Normal 1 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[0] * 3) + 2]; // Normal 1 Z
		currentNormal += 3;
		finalNormals[currentNormal + 0] = normals[(nIndices[1] * 3) + 0]; // Normal 2 X
		finalNormals[currentNormal + 1] = normals[(nIndices[1] * 3) + 1]; // Normal 2 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[1] * 3) + 2]; // Normal 2 Z
		currentNormal += 3;
		finalNormals[currentNormal + 0] = normals[(nIndices[2] * 3) + 0]; // Normal 3 X
		finalNormals[currentNormal + 1] = normals[(nIndices[2] * 3) + 1]; // Normal 3 Y
		finalNormals[currentNormal + 2] = normals[(nIndices[2] * 3) + 2]; // Normal 3 Z
		currentNormal += 3;

		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[0] * 2) + 0]; // UV coord 1 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[0] * 2) + 1]; // UV coord 1 Y
		currentUV += 2;
		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[1] * 2) + 0]; // UV coord 2 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[1] * 2) + 1]; // UV coord 2 Y
		currentUV += 2;
		finalUVCoords[currentUV + 0] = UVcoords[(uIndices[2] * 2) + 0]; // UV coord 3 X
		finalUVCoords[currentUV + 1] = UVcoords[(uIndices[2] * 2) + 1]; // UV coord 3 Y
		currentUV += 2;
	}

	return true;
}

/*
*/
bool COpenGLRenderer::renderObject(
	unsigned int *shaderProgramId, 
	unsigned int *vertexArrayObjectId, 
	int numFaces, 
	GLfloat *objectColor,
	MathHelper::Matrix4 *objectTransformation)
{
	if (m_windowWidth > 0
		&& m_windowHeight > 0
		&& vertexArrayObjectId != NULL 
		&& *vertexArrayObjectId > 0
		&& numFaces > 0
		&& objectColor != NULL
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			m_OpenGLError = true;
			glUseProgram(0);
			return false;
		}

		// Bind vertex array object for this 3D object
		glBindVertexArray((GLuint)*vertexArrayObjectId);

		// ============================================================================================================
		// We're using these same vars for the test object and the actual object, we need to update them in each render pass
		sh_ModelUniformLocation = glGetUniformLocation(*shaderProgramId, "uModelMatrix");
		sh_ViewUniformLocation  = glGetUniformLocation(*shaderProgramId, "uViewMatrix");
		sh_ProjUniformLocation  = glGetUniformLocation(*shaderProgramId, "uProjMatrix");
		sh_colorUniformLocation = glGetUniformLocation(*shaderProgramId, "uColor");

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time
		if (sh_ModelUniformLocation >= 0)
		{
			if (objectTransformation == NULL)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrix(0.0f);
				glUniformMatrix4fv(sh_ModelUniformLocation, 1, GL_FALSE, &(modelMatrix.m[0][0]));
			}
			else
			{
				glUniformMatrix4fv(sh_ModelUniformLocation, 1, GL_FALSE, &(objectTransformation->m[0][0]));
			}
		}

		if (sh_ViewUniformLocation >= 0)
		{
			MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
			glUniformMatrix4fv(sh_ViewUniformLocation, 1, GL_FALSE, &(viewMatrix.m[0][0]));
		}

		if (sh_ProjUniformLocation >= 0)
		{
			MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_windowWidth) / float(m_windowHeight));
			glUniformMatrix4fv(sh_ProjUniformLocation, 1, GL_FALSE, &(projectionMatrix.m[0][0]));
		}

		if (sh_colorUniformLocation >= 0)
		{
			glUniform3f(sh_colorUniformLocation, objectColor[0], objectColor[1], objectColor[2]);
		}

		// Commented out ***, not using indices, we have flattened arrays
		// Draw 
		/* *** glDrawElements(
			GL_TRIANGLES,      // Triangles
			numFaces * 3,      // Number of indices
			GL_UNSIGNED_SHORT, // Data type
			0);*/

		// Draw
		glDrawArrays(
			GL_TRIANGLES,
			0,
			numFaces * 3 // 3 indices per face
		);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");

		// Unbind vertex array object
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
		
		if (!m_OpenGLError)
			return true;
	}

	return false;
}

/*
*/
bool COpenGLRenderer::renderMenuItem(
	unsigned int *shaderProgramId,
	unsigned int *textureObjectId,
	unsigned int *vertexArrayObjectId, 
	int *colorUniformLocation, 
	int *textureUniformLocation,
	GLfloat *menuItemColor
)
{
	if (shaderProgramId != NULL
		&& textureObjectId != NULL
		&& vertexArrayObjectId != NULL
		&& *vertexArrayObjectId > 0
		&& colorUniformLocation != NULL
		&& textureUniformLocation != NULL
		&& *colorUniformLocation >= 0
		&& *textureUniformLocation >= 0
		&& menuItemColor != NULL
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(shaderProgramId))
		{
			m_OpenGLError = true;
			glUseProgram(0);
			return false;
		}

		// Bind vertex array object
		glBindVertexArray(*vertexArrayObjectId);

		// Pass the color value to the uniform
		if (*colorUniformLocation >= 0)
		{
			glUniform3f(*colorUniformLocation, menuItemColor[0], menuItemColor[1], menuItemColor[2]);
		}
		// Set the texture sampler uniform
		if (*textureUniformLocation >= 0 && *textureObjectId >= 0)
		{
			// DO NOT CALL glEnable(GL_TEXTURE_2D) OR OPENGL WILL RETURN AN "1280" ERROR
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *textureObjectId);
			glUniform1i(*textureUniformLocation, 0);
		}

		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			6,                 // Number of indices: 1 face, 2 two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return false;

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);

		return true;
	}

	return false;
}

/*
*/
void COpenGLRenderer::initializeTestObjects()
{
	if (createShaderProgram(
		&mTestshaderProgramID,
		VERTEX_SHADER_TEST_OBJECT,
		FRAGMENT_SHADER_TEST_OBJECT
	))
	{
		useShaderProgram(&mTestshaderProgramID);

		// Get the shader uniform/attribute locations

		// Attributes change per-vertex
		sh_TestPositionAttribLocation = glGetAttribLocation(mTestshaderProgramID, "aPosition");
		sh_TestColorAttribLocation = glGetAttribLocation(mTestshaderProgramID, "aColor");
		// Uniforms change per-object
		sh_ModelUniformLocation = glGetUniformLocation(mTestshaderProgramID, "uModelMatrix");
		sh_ViewUniformLocation = glGetUniformLocation(mTestshaderProgramID, "uViewMatrix");
		sh_ProjUniformLocation = glGetUniformLocation(mTestshaderProgramID, "uProjMatrix");

		// Create and bind a vertex array object
		mVertexPositionArrayObjectID = generateVertexArrayObjectID();

		// Test cube geometry.
		GLfloat vertexPositions[] =
		{
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f
		};

		// Generate a buffer for the vertices and set its data
		glGenBuffers(1, &mVertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(sh_TestPositionAttribLocation);
		glVertexAttribPointer(sh_TestPositionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		GLfloat vertexColors[] =
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		};

		// Generate a buffer for the colors and set its data
		glGenBuffers(1, &mVertexColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		// Link the color buffer with the shader
		glVertexAttribPointer(sh_TestColorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(sh_TestColorAttribLocation);

		short indices[] =
		{
			0, 1, 2, // -x
			1, 3, 2,

			4, 6, 5, // +x
			5, 6, 7,

			0, 5, 1, // -y
			0, 4, 5,

			2, 7, 6, // +y
			2, 3, 7,

			0, 6, 4, // -z
			0, 2, 6,

			1, 7, 3, // +z
			1, 5, 7
		};

		// Generate a buffer for the triangle indices and set its data
		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Unbind vertex array
		glBindVertexArray(0);

		glUseProgram(0);
	}
}

/*
*/
bool COpenGLRenderer::allocateGraphicsMemoryForMenuItem(
	float topX, 
	float topY,
	float menuItemHeight,
	float *uvCoords,
	unsigned int *shaderProgramId,
	unsigned int *vertexArrayObjectID,
	int *colorUniformLocation,
	int *textureUniformLocation)
{
	// Use shader program
	if (shaderProgramId != NULL
		&& vertexArrayObjectID != NULL 
		&& colorUniformLocation != NULL
		&& useShaderProgram(shaderProgramId))
	{
		// Attributes change per-vertex
		GLint posAttribLocation = glGetAttribLocation(*shaderProgramId, "attPosition");
		GLint uvAttribLocation = glGetAttribLocation(*shaderProgramId, "attUV");

		// Get uniforms if Id is invalid
		if ((*colorUniformLocation) < 0)
		{
			*colorUniformLocation = glGetUniformLocation(*shaderProgramId, "uColor");
		}
		if ((*textureUniformLocation) < 0)
		{
			*textureUniformLocation = glGetUniformLocation(*shaderProgramId, "textureSampler");
		}

		m_OpenGLError = checkOpenGLError("COpenGLRenderer::allocateGraphicsMemoryForMenuItem() get Attrib/Uniform locations");
		if (m_OpenGLError)
		{
			return false;
		}

		// Create and bind a vertex array object
		*vertexArrayObjectID = generateVertexArrayObjectID();

		// Single quad representing a menu item
		GLfloat vertexPositions[] =
		{
			-1.0f + topX,  topY - menuItemHeight,  0.0f,  // 0: -x, -y
			-1.0f + topX,  topY,                   0.0f,  // 1: -x, +y
			 1.0f - topX,  topY - menuItemHeight,  0.0f,  // 2: +x, -y
			 1.0f - topX,  topY,                   0.0f   // 3: +x, +y
		};

		// Generate a buffer for the vertices and set its data
		GLuint vertexPosBuffer = 0;
		glGenBuffers(1, &vertexPosBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(posAttribLocation);
		glVertexAttribPointer(posAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("COpenGLRenderer::allocateGraphicsMemoryForMenuItem() pos buffer initialization");

		GLfloat uvData[] =
		{
			uvCoords[4], uvCoords[5], // -x, -y
			uvCoords[0], uvCoords[1], // -x, +y
			uvCoords[6], uvCoords[7], // +x, -y
			uvCoords[2], uvCoords[3]  // +x, +y
		};

		// Generate a buffer for the UV data and fill it
		GLuint uvBuffer = 0;
		glGenBuffers(1, &uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvData), uvData, GL_STATIC_DRAW);
		// Link the vertex position buffer with the shader
		glEnableVertexAttribArray(uvAttribLocation);
		glVertexAttribPointer(uvAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		m_OpenGLError = checkOpenGLError("COpenGLRenderer::allocateGraphicsMemoryForMenuItem() uv buffer initialization");

		short indices[] =
		{
			0, 3, 1, // +z
			0, 2, 3
		};

		// Generate a buffer for the triangle indices and set its data
		GLuint indexBuffer = 0;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_OpenGLError = checkOpenGLError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);");

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete buffers
		deleteBufferObject(&vertexPosBuffer);
		deleteBufferObject(&uvBuffer);
		deleteBufferObject(&indexBuffer);

		glUseProgram(0);

		return true;
	}

	return false;
}

/*
*/
void COpenGLRenderer::renderTestObject(double *deltaTime)
{
	static double green = 0.0f;
	static int dir = 1;
	static double totalDegreesRotated = 0.0;
	static double totalDegreesRotatedRadians = 0.0;
	double degreesToRotate = 0.0;
	double localDeltaTime = 0.01;
	double m_rotationSpeed = 75.0f; //  Object rotation speed (degrees per second)

	// Got delta time from main loop?
	if (deltaTime != NULL)
	{
		localDeltaTime = *deltaTime;

		if (localDeltaTime < 0.0)
		{
			return;
		}
	}

	// Calculate degrees to rotate
	degreesToRotate            = m_rotationSpeed * (localDeltaTime / 1000.0); // degrees = rotation speed * delta time (convert delta time from milliseconds to seconds)
	totalDegreesRotated       += degreesToRotate;	                          // accumulate rotation degrees

	// Reset rotation if needed
	if (totalDegreesRotated > 360.0)
	{
		totalDegreesRotated -= 360.0;
	}

	totalDegreesRotatedRadians = totalDegreesRotated * 3.1459 / 180.0;        // convert total degrees rotated to radians

    // Test, update background color
	{
		double colorCycleSpeed = 0.5;
		double deltaGreen = colorCycleSpeed * (localDeltaTime / 1000.0);

		green += deltaGreen * dir;

		if (green > 1.0)
		{
			green = 1.0;
			dir = -1;
		}
		else if (green < 0.0)
		{
			green = 0.0;
			dir = 1;
		}

		glClearColor(0.25f, (float)(green), 0.75f, 1.f);
	}

	if (m_windowWidth > 0 
		&& m_windowHeight > 0 
		&& !m_OpenGLError)
	{
		if (!useShaderProgram(&mTestshaderProgramID))
		{
			m_OpenGLError = true;
			glUseProgram(0);
			return;
		}

		// BIND VERTEX ARRAY OBJECT !
		// ============================================================================================================
		glBindVertexArray(mVertexPositionArrayObjectID);

		// ====== Update Model View Projection matrices and pass them to the shader====================================
		// This needs to be done per-frame because the values change over time
		sh_ModelUniformLocation = glGetUniformLocation(mTestshaderProgramID, "uModelMatrix");
		sh_ViewUniformLocation  = glGetUniformLocation(mTestshaderProgramID, "uViewMatrix");
		sh_ProjUniformLocation  = glGetUniformLocation(mTestshaderProgramID, "uProjMatrix");

		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrix((float)(totalDegreesRotatedRadians));
		glUniformMatrix4fv(sh_ModelUniformLocation, 1, GL_FALSE, &(modelMatrix.m[0][0]));

		MathHelper::Matrix4 viewMatrix = MathHelper::SimpleViewMatrix(m_cameraDistance);
		glUniformMatrix4fv(sh_ViewUniformLocation, 1, GL_FALSE, &(viewMatrix.m[0][0]));

		MathHelper::Matrix4 projectionMatrix = MathHelper::SimpleProjectionMatrix(float(m_windowWidth) / float(m_windowHeight));
		glUniformMatrix4fv(sh_ProjUniformLocation, 1, GL_FALSE, &(projectionMatrix.m[0][0]));

		// ====== DRAW ================================================================================================
		
		// Draw 
		glDrawElements(
			GL_TRIANGLES,      // Triangles
			(6 * 2) * 3,       // Number of indices: 36 indices (six faces, two triangles per face, 3 indices per triangle)
			GL_UNSIGNED_SHORT, // Data type
			0);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("glDrawElements(GL_TRIANGLES)");
		if (m_OpenGLError)
			return;		

		// Unbind vertex array
		glBindVertexArray(0);

		// Unbind shader program
		glUseProgram(0);
	}
}

/*
*/
bool COpenGLRenderer::checkOpenGLError(char *operationAttempted)
{
	bool errorDetected = false;

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error on " <<  operationAttempted << ": " << err << endl;
		errorDetected = true;
	}

	return errorDetected;
}

/*
*/
void COpenGLRenderer::moveCamera(float direction)
{
	m_cameraDistance += (direction * MOVE_CAMERA_DELTA);

	if (m_cameraDistance > MAX_CAMERA_DISTANCE)
	{
		m_cameraDistance = MAX_CAMERA_DISTANCE;
	}
	else if (m_cameraDistance < MIN_CAMERA_DISTANCE)
	{
		m_cameraDistance = MIN_CAMERA_DISTANCE;
	}
}

/*
*/
void COpenGLRenderer::deleteTexture(unsigned int *id) 
{
	if (id != NULL && *id > 0) 
	{ 
		glDeleteTextures(1, id);
	} 
}

