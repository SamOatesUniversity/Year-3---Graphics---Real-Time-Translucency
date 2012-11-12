#include "Shader.h"

/*
* \brief Class constructor
*/
Shader::Shader() :
	m_vertex_shader(NULL),
	m_fragment_shader(NULL),
	m_program(NULL)
{
}

/*
* \brief Class destructor
*/
Shader::~Shader()
{
	glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
    glDeleteProgram(m_program);
}


/*
* \brief Load our shaders into our shader object
*/
const bool Shader::Load(
		const char *vertexShader,							//!< Name of the vertex shader
		const char *fragmentShader							//!< Name of the fragment shader
	)
{
	m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertex_shader_string = tyga::stringFromFile(vertexShader);
    const char *vertex_shader_code = vertex_shader_string.c_str();

    glShaderSource(
		m_vertex_shader,
		1,
        (const GLchar **) &vertex_shader_code, 
		NULL
	);

	int compile_status = 0;
    glCompileShader(m_vertex_shader);
    glGetShaderiv(
		m_vertex_shader,
        GL_COMPILE_STATUS,
		&compile_status
	);

    if (compile_status != GL_TRUE) {
        const int string_length = 1024;
        GLchar log[string_length]= "";
        glGetShaderInfoLog(
			m_vertex_shader,
            string_length,
            NULL,
            log
		);
        std::cerr << log << std::endl;
		return false;
    }
 
    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragment_shader_string = tyga::stringFromFile(fragmentShader);
    const char *const fragment_shader_code = fragment_shader_string.c_str();

    glShaderSource(
		m_fragment_shader, 
		1,
		(const GLchar **) &fragment_shader_code, 
		NULL
	);

    glCompileShader(m_fragment_shader);
    
	glGetShaderiv(
		m_fragment_shader,
        GL_COMPILE_STATUS,
        &compile_status
	);
    
	if (compile_status != GL_TRUE) {
        const int string_length = 1024;
        GLchar log[string_length]= "";
        glGetShaderInfoLog(
			m_fragment_shader,
            string_length,
            NULL,
            log
		);

        std::cerr << log << std::endl;
		return false;
    }

	m_program = glCreateProgram();

	return true;
}