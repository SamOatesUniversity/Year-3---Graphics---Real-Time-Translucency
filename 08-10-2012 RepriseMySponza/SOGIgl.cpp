#include "SOGIgl.h"

static const int string_length = 1024;

/*
*	\brief 
*/
const bool loadVertexShader(
		char *shader,				//!<
		GLuint &vertexShader		//!<
	)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::string vertex_shader_string = tyga::stringFromFile(shader);
    const char *vertex_shader_code = vertex_shader_string.c_str();

    glShaderSource(
		vertexShader, 
		1,
        (const GLchar **) &vertex_shader_code, 
		NULL
	);

    glCompileShader(vertexShader);

	GLint compile_status = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile_status);

    if (compile_status != GL_TRUE) {
        GLchar log[string_length]= "";
        glGetShaderInfoLog(vertexShader, string_length, NULL, log);
        std::cerr << log << std::endl;
		return false;
    }

	return true;
}

/*
*	\brief 
*/
const bool loadFragmentShader(
		char *shader,				//!< 
		GLuint &fragmentShader		//!< 
	)
{
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string fragment_shader_string = tyga::stringFromFile(shader);
    const char *fragment_shader_code = fragment_shader_string.c_str();

    glShaderSource(
		fragmentShader, 
		1,
        (const GLchar **) &fragment_shader_code, 
		NULL
	);

    glCompileShader(fragmentShader);

	GLint compile_status = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        GLchar log[string_length]= "";
        glGetShaderInfoLog(fragmentShader, string_length, NULL, log);
        std::cerr << log << std::endl;
		return false;
    }

	return true;
}