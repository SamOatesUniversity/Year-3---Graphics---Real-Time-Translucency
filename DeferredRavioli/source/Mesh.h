#pragma once

#include "tgl.h"
#include <glm/glm.hpp>

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
												GLuint &vertexVBO,				//!< 
												GLuint &elementVBO,				//!< 
												int &elementCount,				//!< 
												GLuint &vao						//!< 
											);

};