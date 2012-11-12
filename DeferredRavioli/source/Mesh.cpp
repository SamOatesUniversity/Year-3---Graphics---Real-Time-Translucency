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
		GLuint &vertexVBO,						//!< 
		GLuint &elementVBO,						//!<
		int &elementCount,						//!<
		GLuint &vao								//!<  
	)
{
	m_vertexVBO = vertexVBO;
	m_elementVBO = elementVBO;
	m_vao = vao;
	m_elementCount = elementCount;
}

