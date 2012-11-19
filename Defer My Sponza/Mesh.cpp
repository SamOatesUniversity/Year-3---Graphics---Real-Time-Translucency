#include "Mesh.h"

/*
*	\brief Class constructor 
*/
Mesh::Mesh() :
		m_elementCount(0),
		m_elementVBO(NULL),
		m_vertexVBO(NULL),
		m_vao(NULL)
{

}

/*
*	\brief Class destructor
*/
Mesh::~Mesh()
{

}

/*
*	\brief Create the mesh
*/
void Mesh::Create( 
		Vertex *vertices,								//! A pointer to an array of verticies
		const int noofVerticies,						//! The number of verticies in the above array
		unsigned int *elements,							//! A pointer to an array of elements
		const int noofElements							//! The number of elements in the above array
	)
{
	// verticies
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		noofVerticies * sizeof(Vertex),
		vertices,
		GL_STATIC_DRAW
		);

	// positon
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE,
		sizeof(Vertex), 
		TGL_BUFFER_OFFSET(0)
		);

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 
		3, 
		GL_FLOAT, 
		GL_TRUE,
		sizeof(Vertex), 
		TGL_BUFFER_OFFSET(3 * sizeof(float))
		);

	//// tangent
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(
	//	2, 
	//	3, 
	//	GL_FLOAT, 
	//	GL_FALSE,
	//	sizeof(Vertex), 
	//	TGL_BUFFER_OFFSET(6 * sizeof(float))
	//	);

	//// texcoords
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(
	//	3, 
	//	2, 
	//	GL_FLOAT, 
	//	GL_FALSE,
	//	sizeof(Vertex), 
	//	TGL_BUFFER_OFFSET(9 * sizeof(float))
	//	);

	// elements
	glGenBuffers(1, &m_elementVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementVBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		noofElements * sizeof(unsigned int),
		elements,
		GL_STATIC_DRAW
		);

	m_elementCount = noofElements;
}

/*
*	\brief Release the OpenGL buffers associated with this mesh
*/
void Mesh::Release()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteBuffers(1, &m_elementVBO);
	glDeleteVertexArrays(1, &m_vao);
}

/*
*	\brief Draw the mesh
*/
void Mesh::Draw() const
{
	glBindVertexArray(m_vao);					
	glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

