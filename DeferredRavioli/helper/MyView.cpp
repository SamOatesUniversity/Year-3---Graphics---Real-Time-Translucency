#include "MyView.hpp"
#include "FileHelper.hpp"
#include "TcfScene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/*
*	\brief Class constructor
*/
MyView::MyView() :
		m_camera(nullptr)
{
    m_scene.reset(new TcfScene());
}

/*
*	\brief Class destructor
*/
MyView::~MyView() 
{

}

/*
*	\brief Reload the programs shaders
*/
void MyView::reloadShaders()
{
	for( std::map<char*, Shader*>::iterator shaderIt = m_shader.begin(); shaderIt != m_shader.end(); ++shaderIt)
	{
		delete (*shaderIt).second;
	}
	m_shader.clear();

	// Create an ambient shader
	Shader *const ambiant = new Shader();
	if (!ambiant->Load("shaders/ambient_vs.glsl", "shaders/ambient_fs.glsl")) 
	{
		std::cout << "Failed to load the ambiant shader!" << std::endl;
		system("PAUSE");
	}
	else
	{
		glAttachShader(ambiant->GetProgram(), ambiant->GetVertexShader());
		glBindAttribLocation(ambiant->GetProgram(), 0, "vertex_position");

		glAttachShader(ambiant->GetProgram(), ambiant->GetFragmentShader());
		glBindFragDataLocation(ambiant->GetProgram(), 0, "fragment_colour");

		glLinkProgram(ambiant->GetProgram());

		std::cout << "Loaded ambient shader..." << std::endl;

		m_shader["ambiant"] = ambiant;
	}	


	// Create the shader which will output the world position and normal to the gbuffer
	Shader *const gbuffer = new Shader();
	if (!gbuffer->Load("shaders/gbuffer_vs.glsl", "shaders/gbuffer_fs.glsl"))
	{
		std::cout << "Failed to load the gbuffer shader!" << std::endl;
		system("PAUSE");
	}
	else
	{
		glAttachShader(gbuffer->GetProgram(), gbuffer->GetVertexShader());
		glBindAttribLocation(gbuffer->GetProgram(), 0, "vertex_position");
		glBindAttribLocation(gbuffer->GetProgram(), 1, "vertex_normal");

		glAttachShader(gbuffer->GetProgram(), gbuffer->GetFragmentShader());
		glBindFragDataLocation(gbuffer->GetProgram(), 0, "fragment_colour");

		glLinkProgram(gbuffer->GetProgram());

		std::cout << "Loaded gbuffer shader..." << std::endl;

		m_shader["gbuffer"] = gbuffer;
	}
}

/*
*	\brief Create the GBuffer
*/
void MyView::CreateGBuffer(
		int windowWidth,
		int windowHeight
	)
{
	glGenFramebuffers(1, &m_gbuffer.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer.frameBuffer);

	glGenTextures(GBufferTexture::noof, m_gbuffer.texture);
	glGenTextures(1, &m_gbuffer.depth);

	// setup colour buffers
	for (unsigned int textureIndex = 0; textureIndex < GBufferTexture::noof; ++textureIndex) {
		glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex]);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureIndex, GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex], 0);
	}

	// setup depth
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.depth);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE, m_gbuffer.depth, 0);
	
	GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBufs);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to create GBuffer!" << std::endl;
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

/*
*	\brief Called on window creation
*/
void MyView::windowViewWillStart(
	std::shared_ptr<tyga::Window> window					//!< A pointer to the window object
	)
{
	m_startTime = std::chrono::system_clock::now();

	reloadShaders();

	if (!m_scene->readFile("sponza.tcf")) {
		std::cerr << "Failed to read sponza.tcf data file" << std::endl;
	}

	{
		std::vector<glm::vec2> vertices(4);
		vertices[0] = glm::vec2(-1, -1);
		vertices[1] = glm::vec2(1, -1);
		vertices[2] = glm::vec2(1, 1);
		vertices[3] = glm::vec2(-1, 1);

		glGenBuffers(1, &m_meshQuad.getVertexVBO());
		glBindBuffer(GL_ARRAY_BUFFER, m_meshQuad.getVertexVBO());
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(glm::vec2),
			vertices.data(),
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_meshQuad.getVAO());
		glBindVertexArray(m_meshQuad.getVAO());
		glBindBuffer(GL_ARRAY_BUFFER, m_meshQuad.getVertexVBO());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// Load the scene
	const int noofMeshes = m_scene->meshCount();
	for (int meshIndex = 0; meshIndex < noofMeshes; ++meshIndex)
	{
		const TcfScene::Mesh sceneMesh = m_scene->mesh(meshIndex);

		// populate the vertex array
		const int noofVerticies = sceneMesh.position_array.size();
		Vertex *const vertices = new Vertex[noofVerticies];
		for (int vertexIndex = 0; vertexIndex < noofVerticies; ++vertexIndex)
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
		mesh.Create(vertices, noofVerticies, elements, noofElements);
		m_meshes.push_back(mesh);

		delete[] vertices;
		delete[] elements;
	}

	m_camera = new Camera();
}

/*
*	\brief Called when the window has reset (normally caused by a resize) 
*/
void MyView::windowViewDidReset(
		std::shared_ptr<tyga::Window> window,				//!< A pointer to the window object
		int width,											//!< The new width of the window
        int height											//!< The new height of the window
	)
{
    glViewport(0, 0, width, height);

	CreateGBuffer(width, height);

	// create light buffer
	glGenFramebuffers(1, &m_lbuffer.frameBuffer);
	glGenRenderbuffers(1, &m_lbuffer.colorBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lbuffer.colorBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_lbuffer.colorBuffer);

	const GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH, "framebuffer not complete");
	}

}

/*
*	\brief Called when the window is shutting down
*/
void MyView::windowViewDidStop(
		std::shared_ptr<tyga::Window> window				//!< A pointer to the window object
	)
{
	for (Mesh mesh : m_meshes)
		mesh.Release();

	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	for( std::map<char*, Shader*>::iterator shaderIt = m_shader.begin(); shaderIt != m_shader.end(); ++shaderIt)
		delete (*shaderIt).second;
	m_shader.clear();
}

/*
*	\brief Called when the scene should render
*/
void MyView::windowViewRender(
		std::shared_ptr<tyga::Window> window				//!< A pointer to the window object
	)
{
	GLint viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	const auto clock_time = std::chrono::system_clock::now() - m_startTime;
    const auto clock_millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float time_seconds = clock_millisecs.count() * 0.001f;

	//////////////////////////////////////////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer.frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);

	glUseProgram(m_shader["gbuffer"]->GetProgram());

	glUniformMatrix4fv(glGetUniformLocation(m_shader["gbuffer"]->GetProgram(), "viewMatrix"), 1, GL_FALSE, &m_camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_shader["gbuffer"]->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &m_camera->GetProjectionMatrix(aspect_ratio)[0][0]); 

	const int noofModels = m_scene->modelCount();
	for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
	{
		const TcfScene::Model model = m_scene->model(modelIndex);
		const Mesh mesh = m_meshes[model.mesh_index];

		glUniformMatrix4fv(glGetUniformLocation(m_shader["gbuffer"]->GetProgram(), "worldMatrix"), 1, GL_FALSE, &model.xform[0][0]);
		mesh.Draw();		
	}

	//////////////////////////////////////////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);

	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glUseProgram(m_shader["ambiant"]->GetProgram());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[GBufferTexture::position]);
	glUniform1i(glGetUniformLocation(m_shader["ambiant"]->GetProgram(), "sampler_world_position"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::normal]);
	glUniform1i(glGetUniformLocation(m_shader["ambiant"]->GetProgram(), "sampler_world_normal"), 1);

	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, GL_MAX_ELEMENTS_VERTICES);

	glDisable(GL_STENCIL_TEST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_lbuffer.frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(
		0, 0, viewport_size[2], viewport_size[3],
		0, 0, viewport_size[2], viewport_size[3],
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}
