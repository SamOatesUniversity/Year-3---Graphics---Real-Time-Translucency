#pragma once

#include "tgl.h"
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;												//!< The position of the vertex
	glm::vec3 normal;												//!< The normal of the vertex
	glm::vec3 tangent;												//!< The tangent of the vertex
	glm::vec2 texcoord;												//!< The texcoords of the vertex
};

class Mesh {

private:

	GLuint									m_vertexVBO;						//!< 
	GLuint									m_elementVBO;						//!< 
	GLuint									m_vao;								//!< 

	int										m_elementCount;						//!< 
	int										m_vertexCount;						//!< 

public:
											//! Class constructor
											Mesh();

											//! Class destructor
											~Mesh();

											//! Initialize vertex and element counts, if the mesh is externally initialized
	void									Create(
												const int noofVerticies,		//! The number of verticies in the vertex array
												const int noofElements			//! The number of elements in the element array
											);

											//! Create the mesh
	void									Create(
												Vertex *vertices,				//! A pointer to an array of verticies
												const int noofVerticies,		//! The number of verticies in the above array
												unsigned int *elements,			//! A pointer to an array of elements
												const int noofElements			//! The number of elements in the above array
											);

											//! Release the OpenGL buffers associated with this mesh
	void									Release();

											//! Draw the mesh
	void									Draw() const;

											//! Get the number of vertices in the mesh
	const int								&GetNoofVertices() const
											{
												return m_vertexCount;
											}

											//! Get the number of elements in the mesh
	const int								&GetNoofElements() const
											{
												return m_elementCount;
											}

											//! Get the meshes vertex vbo
	GLuint									&getVertexVBO()
											{
												return m_vertexVBO;
											}

											//! Get the meshes element vbo
	GLuint									&getElementVBO()
											{
												return m_elementVBO;
											}

											//! Get a reference of the meshes vao
	GLuint									&getVAO()
											{
												return m_vao;
											}

};