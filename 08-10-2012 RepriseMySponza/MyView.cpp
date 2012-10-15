#include "MyView.hpp"
#include "FileHelper.hpp"
#include "TcfScene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

MyView::MyView() :	m_camera(nullptr)
{
    scene_.reset(new TcfScene());
	m_shadow.texture = NULL;
}

MyView::~MyView() 
{
	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}

void MyView::windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
    start_time_ = std::chrono::system_clock::now();

    reloadShaders();

    if (!scene_->readFile("sponza.tcf")) {
        std::cerr << "Failed to read sponza.tcf data file" << std::endl;
    }

	// Create a texture to render too
	//Size of shadow map
	static const int shadowMapSize = 512;

	//Create the shadow map texture
	glGenTextures(1, &m_shadow.texture);
    glBindTexture(GL_TEXTURE_2D, m_shadow.texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_DEPTH_COMPONENT, 
		shadowMapSize, 
		shadowMapSize, 
		0,
        GL_DEPTH_COMPONENT, 
		GL_UNSIGNED_BYTE, 
		NULL
	);

	glGenFramebuffers(1, &m_shadow.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadow.frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadow.texture, 0);


	// Load the scene
	const int noofMeshes = scene_->meshCount();
	for (int meshIndex = 0; meshIndex < noofMeshes; ++meshIndex)
	{
		const TcfScene::Mesh sceneMesh = scene_->mesh(meshIndex);
		
		// populate the vertex array
		const int noofverticies = sceneMesh.position_array.size();
		Vertex *const vertices = new Vertex[noofverticies];
		for (int vertexIndex = 0; vertexIndex < noofverticies; ++vertexIndex)
		{
			vertices[vertexIndex].position = sceneMesh.position_array[vertexIndex];
			vertices[vertexIndex].normal = sceneMesh.normal_array[vertexIndex];
		}

		// populate elements (index) array
		const unsigned int noofElements = sceneMesh.element_array.size();
		unsigned int *const elements = new unsigned int[noofElements];
		for (unsigned int elementIndex = 0; elementIndex < noofElements; ++elementIndex) 
		{
			elements[elementIndex] = sceneMesh.element_array[elementIndex];
		}

		// make our new mesh
		Mesh mesh;		

		// verticies
		glGenVertexArrays(1, &mesh.vao);
		glBindVertexArray(mesh.vao);
		glGenBuffers(1, &mesh.vertex_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			noofverticies * sizeof(Vertex),
			vertices,
			GL_STATIC_DRAW
		);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0, 
			3, 
			GL_FLOAT, 
			GL_FALSE,
			sizeof(Vertex), 
			TGL_BUFFER_OFFSET(0)
		);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1, 
			3, 
			GL_FLOAT, 
			GL_TRUE,
			sizeof(Vertex), 
			TGL_BUFFER_OFFSET(3 * sizeof(float))
		);

		// elements
		glGenBuffers(1, &mesh.element_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.element_vbo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			noofElements * sizeof(unsigned int),
			elements,
			GL_STATIC_DRAW
		);
		mesh.element_count = noofElements;

		meshes_.push_back(mesh);

		delete vertices;
		delete elements;

	}

    glEnable(GL_DEPTH_TEST);

	m_camera = new CCamera();
}

void MyView::reloadShaders()
{
    if (shading_.vertex_shader != 0) {
        glDeleteShader(shading_.vertex_shader);
    }
    if (shading_.fragment_shader != 0) {
        glDeleteShader(shading_.fragment_shader);
    }
    if (shading_.program != 0) {
        glDeleteProgram(shading_.program);
    }

	if (m_shadow.shader.vertex_shader)
		glDeleteShader(m_shadow.shader.vertex_shader);
	if (m_shadow.shader.vertex_shader)
		glDeleteShader(m_shadow.shader.fragment_shader);
	if (m_shadow.shader.vertex_shader)
		glDeleteProgram(m_shadow.shader.program);
    
	// scene shaders

	loadVertexShader("reprise_vs.glsl", shading_.vertex_shader);
	loadFragmentShader("reprise_fs.glsl", shading_.fragment_shader);
  
    shading_.program = glCreateProgram();
    glAttachShader(shading_.program, shading_.vertex_shader);
	glBindAttribLocation(shading_.program, 0, "vertex_position");
	glBindAttribLocation(shading_.program, 1, "vertex_normal");

    glAttachShader(shading_.program, shading_.fragment_shader);
	glBindFragDataLocation(shading_.program, 0, "fragment_colour");

    glLinkProgram(shading_.program);

	// shadow shaders
	loadVertexShader("shadow_vs.glsl", m_shadow.shader.vertex_shader);
	loadFragmentShader("shadow_fs.glsl", m_shadow.shader.fragment_shader);

	m_shadow.shader.program = glCreateProgram();
    glAttachShader(m_shadow.shader.program, m_shadow.shader.vertex_shader);
	glAttachShader(m_shadow.shader.program, m_shadow.shader.fragment_shader);
	glLinkProgram(m_shadow.shader.program);

}

void MyView::windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
    if (shading_.vertex_shader != 0) {
        glDeleteShader(shading_.vertex_shader);
    }
    if (shading_.fragment_shader != 0) {
        glDeleteShader(shading_.fragment_shader);
    }
    if (shading_.program != 0) {
        glDeleteProgram(shading_.program);
    }

	if (m_shadow.shader.vertex_shader)
		glDeleteShader(m_shadow.shader.vertex_shader);
	if (m_shadow.shader.vertex_shader)
		glDeleteShader(m_shadow.shader.fragment_shader);
	if (m_shadow.shader.vertex_shader)
		glDeleteProgram(m_shadow.shader.program);

	for (Mesh mesh : meshes_)
	{
		glDeleteBuffers(1, &mesh.vertex_vbo);
		glDeleteBuffers(1, &mesh.element_vbo);
		glDeleteVertexArrays(1, &mesh.vao);
	}

}

void MyView::windowViewRender(std::shared_ptr<tyga::Window> window)
{
	m_camera->Update();

	GLint viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	const auto clock_time = std::chrono::system_clock::now() - start_time_;
    const auto clock_millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float time_seconds = 0.001f * clock_millisecs.count();

	GLint link_status;

	// draw the scene
	{
		glClearColor(0.f, 0.f, 0.25f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glGetProgramiv(m_shadow.shader.program, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
			return;

		glUseProgram(m_shadow.shader.program);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow.frameBuffer);



		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		// ******************************************************************************

		glGetProgramiv(shading_.program, GL_LINK_STATUS, &link_status);
		if (link_status != GL_TRUE)
			return;

		glUseProgram(shading_.program);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow.frameBuffer);

		const glm::mat4 projectionMatrix = glm::perspective(45.0f, aspect_ratio, 0.1f, 1000.f);
		glUniformMatrix4fv(glGetUniformLocation(shading_.program, "viewMatrix"), 1, GL_FALSE, &m_camera->GetViewMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shading_.program, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]); 

		const int noofModels = scene_->modelCount();
		for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
		{
			const TcfScene::Model model = scene_->model(modelIndex);
			const Mesh mesh = meshes_[model.mesh_index];

			glUniformMatrix4fv(glGetUniformLocation(shading_.program, "worldMatrix"), 1, GL_FALSE, &model.xform[0][0]);

			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.element_count, GL_UNSIGNED_INT, 0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}
}
