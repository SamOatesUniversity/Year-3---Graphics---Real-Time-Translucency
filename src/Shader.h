#pragma once

#include "tgl.h"

#include <glm/glm.hpp>
#include "FileHelper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Shader
{
private:
	GLuint							m_vertex_shader;							//!< Vertex shader ID
	GLuint							m_fragment_shader;							//!< Fragment shader ID
	GLuint							m_program;									//!< Program ID

public:
									//! Class constructor
									Shader();

									//! Class destructor
									~Shader();

									//! Load our shaders into our shader wrapper
	const bool						Load(
										const char *vertexShader,				//!< Name of the vertex shader 
										const char *fragmentShader				//!< Name of the fragment shader
									);

									//! Process the shader code handling #includes ect...
	const bool						ProcessShaderCode(
										std::string &shadercode
									);

									//! Get the shader program
	const GLuint					GetProgram() const
									{
										return m_program;
									}

									//! Get the vertex shader
	const GLuint					GetVertexShader() const
									{
										return m_vertex_shader;
									}

									//! Get the fragment shader
	const GLuint					GetFragmentShader() const
									{
										return m_fragment_shader;
									}
};

