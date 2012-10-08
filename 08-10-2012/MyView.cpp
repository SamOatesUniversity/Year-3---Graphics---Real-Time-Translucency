#include "MyView.hpp"
#include "FileHelper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

MyView::
	MyView()
{
}

MyView::
	~MyView() {
}

void MyView::
	windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
	/*
	* Tutorial: This function is where we prepare for using OpenGL and prepare
	*           for drawing.  Usually we will create OpenGL resources here.
	*/

	start_time_ = std::chrono::system_clock::now();

	/*
	* Tutorial: Creating the shader resources is done in a separate function
	*           because this app allows them to be reloaded at runtime.
	*/

	reloadShaders();

	/*
	* Tutorial: Generate the geometry data (vertex and element data). Often
	*           this data will come from a data file.
	*/

	const unsigned int num_of_triangles = 5;
	const unsigned int num_of_vertices = num_of_triangles + 1;
	Vertex vertices[num_of_vertices];
	for (unsigned int i=0; i<num_of_triangles; ++i) {
		const float radius = 0.5f;
		const float angle_radians = 6.283f * (float)i / (float)num_of_triangles;
		vertices[i].position = glm::vec3(radius * cos(angle_radians),
			radius * sin(angle_radians),
			0);
	}
	vertices[num_of_triangles].position = glm::vec3(0, 0, 0);

	const unsigned int num_of_elements = num_of_triangles * 3;
	unsigned int elements[num_of_elements];
	for (unsigned int i=0; i<num_of_triangles; ++i) {
		elements[3*i+0] = num_of_triangles;
		elements[3*i+1] = i;
		elements[3*i+2] = (i+1) % num_of_triangles;
	}

	/*
	* Tutorial: Create OpenGL buffers to hold the geometry data. This is
	*           how we pass our geometry data to OpenGL so it can draw it.
	*/

	glGenVertexArrays(1, &mesh_.vao);
	glBindVertexArray(mesh_.vao);
	glGenBuffers(1, &mesh_.vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_.vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		num_of_vertices * sizeof(Vertex),
		vertices,
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), TGL_BUFFER_OFFSET(0));
	glGenBuffers(1, &mesh_.element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_.element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		num_of_elements * sizeof(unsigned int),
		elements,
		GL_STATIC_DRAW);
	mesh_.element_count = num_of_elements;
	glBindVertexArray(0);

	/*
	* Tutorial: We can also configure OpenGL settings now the window and
	*           OpenGL context has been created.
	*/

	glEnable(GL_DEPTH_TEST);
}

void MyView::
	reloadShaders()
{
	/*
	* Tutorial: This function could be called many times so it is necessary
	*           to ensure any previously created shaders and programs are
	*           deleted before attempting to reload the new ones.
	*/

	if (first_.vertex_shader != 0) {
		glDeleteShader(first_.vertex_shader);
	}
	if (first_.fragment_shader != 0) {
		glDeleteShader(first_.fragment_shader);
	}
	if (first_.program != 0) {
		glDeleteProgram(first_.program);
	}

	/*
	* Tutorial: Shaders are loaded from a text file into a string which is
	*           passed to OpenGL for compiling.  Compile errors can be
	*           queried via the info log.
	*/

	GLint compile_status = 0;

	first_.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_string = tyga::stringFromFile("first_vs.glsl");
	const char *vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(first_.vertex_shader, 1,
		(const GLchar **) &vertex_shader_code, NULL);
	glCompileShader(first_.vertex_shader);
	glGetShaderiv(first_.vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length]= "";
		glGetShaderInfoLog(first_.vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	first_.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_string = tyga::stringFromFile("first_fs.glsl");
	const char *fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(first_.fragment_shader, 1,
		(const GLchar **) &fragment_shader_code, NULL);
	glCompileShader(first_.fragment_shader);
	glGetShaderiv(first_.fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		const int string_length = 1024;
		GLchar log[string_length]= "";
		glGetShaderInfoLog(first_.fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	/*
	* Tutorial: A program object is made to host the shaders and their
	*           settings. This program object attempts to bind the shaders
	*           together when it is linked. If the shaders aren't compatible
	*           the link process will fail.
	*/

	first_.program = glCreateProgram();
	glAttachShader(first_.program, first_.vertex_shader);
	glBindAttribLocation(first_.program, 0, "vertex_position");
	glAttachShader(first_.program, first_.fragment_shader);
	glBindFragDataLocation(first_.program, 0, "fragment_colour");
	glLinkProgram(first_.program);

	/*
	* Tutorial: Usually you would check the link was successful.  This app
	*           does that in the windowViewRender method as discussed there.
	*/
}

void MyView::
	windowViewDidReset(std::shared_ptr<tyga::Window> window,
	int width,
	int height)
{
	/*
	* Tutorial: This is where we respond to the window resizing. At a minimum
	*           we tell OpenGL to use the entire area of the resized window.
	*/

	glViewport(0, 0, width, height);
}

void MyView::
	windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
	/*
	* Tutorial: This is where we must free any OpenGL resources we've created.
	*/

	if (first_.vertex_shader != 0) {
		glDeleteShader(first_.vertex_shader);
	}
	if (first_.fragment_shader != 0) {
		glDeleteShader(first_.fragment_shader);
	}
	if (first_.program != 0) {
		glDeleteProgram(first_.program);
	}
	glDeleteBuffers(1, &mesh_.vertex_vbo);
	glDeleteBuffers(1, &mesh_.element_vbo);
	glDeleteVertexArrays(1, &mesh_.vao);
}

void MyView::
	windowViewRender(std::shared_ptr<tyga::Window> window)
{
	/*
	* Tutorial: Erase the image of the previous frame ready to render this one.
	*/

	glClearColor(0.f, 0.f, 0.25f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	* Tutorial: In this example app the shaders can be reloaded at runtime, so
	*           it is necessary to check if they compiled and linked
	*           successfully before rendering with them. Not usually necessary.
	*/

	GLint link_status;
	glGetProgramiv(first_.program, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		return;
	}

	/*
	* Tutorial: Use our shader program and populate its uniform variables.
	*/

	glUseProgram(first_.program);

	const auto clock_time = std::chrono::system_clock::now() - start_time_;
	const auto clock_millisecs
		= std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
	const float time_seconds = 0.001f * clock_millisecs.count();

	const float angle_radians = time_seconds;
	const float radius = 0.25f;
	glm::mat4 mystery_xform
		= glm::translate(glm::mat4(1),
		glm::vec3(radius * std::cos(angle_radians),
		radius * std::sin(angle_radians),
		0));

	glUniform1f(
		glGetUniformLocation(first_.program, "time_seconds"),
		time_seconds);
	glUniformMatrix4fv(
		glGetUniformLocation(first_.program, "mystery_xform"),
		1, GL_FALSE, glm::value_ptr(mystery_xform));

	/*
	* Tutorial: Bind (attach to the pipeline) our geometry and draw it.
	*/

	glBindVertexArray(mesh_.vao);
	glDrawElements(GL_TRIANGLES, mesh_.element_count, GL_UNSIGNED_INT, 0);
}
