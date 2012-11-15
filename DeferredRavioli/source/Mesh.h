#pragma once

#include "tgl.h"
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;												//!< The position of the vertex
	glm::vec3 normal;												//!< The normal of the vertex
};

class Mesh {

private:

	GLuint									m_vertexVBO;						//!< 
	GLuint									m_elementVBO;						//!< 
	GLuint									m_vao;								//!< 

	int										m_elementCount;						//!< 

public:
											//! Class constructor
											Mesh();

											//! Class destructor
											~Mesh();

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

											//! Get the meshes vertex vbo
	GLuint									&getVertexVBO()
											{
												return m_vertexVBO;
											}

											//! Get the meshes vao
	GLuint									&getVAO()
											{
												return m_vao;
											}

};