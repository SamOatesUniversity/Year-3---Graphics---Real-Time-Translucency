#pragma once

#include <FileHelper.hpp>
#include <tgl.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>

							//!< Load a vertex shader
const bool					loadVertexShader(
								char *shader,					//!< Name of the shader
								GLuint &vertexShader			//!< Reference of the vertex shader ID
							);

							//!< Load a fragment shader
const bool					loadFragmentShader(
								char *shader,					//!< Name of the shader
								GLuint &fragmentShader			//!< Reference of the vertex shader ID
							);