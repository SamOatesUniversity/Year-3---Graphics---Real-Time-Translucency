#include "MyView.hpp"
#include "FileHelper.hpp"
#include <tsl/primitives.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>

MyView::
MyView()
{
	m_gbuffer.frameBuffer = NULL;
	m_lbuffer.frameBuffer = NULL;
}

MyView::
~MyView() {
}

void MyView::
setScene(std::shared_ptr<const MyScene> scene)
{
    scene_ = scene;
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

	// Create an ambient shader
	Shader *const pointlight = new Shader();
	if (!pointlight->Load("shaders/pointlight_vs.glsl", "shaders/pointlight_fs.glsl")) 
	{
		std::cout << "Failed to load the pointlight shader!" << std::endl;
		system("PAUSE");
	}
	else
	{
		glAttachShader(pointlight->GetProgram(), pointlight->GetVertexShader());
		glBindAttribLocation(pointlight->GetProgram(), 0, "vertex_position");

		glAttachShader(pointlight->GetProgram(), pointlight->GetFragmentShader());
		glBindFragDataLocation(pointlight->GetProgram(), 0, "fragment_colour");

		glLinkProgram(pointlight->GetProgram());

		std::cout << "Loaded pointlight shader..." << std::endl;

		m_shader["pointlight"] = pointlight;
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

	GLenum drawBufs[GBufferTexture::noof];

	// setup colour buffers
	for (unsigned int textureIndex = 0; textureIndex < GBufferTexture::noof; ++textureIndex) {
		glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex]);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureIndex, GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex], 0);
		drawBufs[textureIndex] = GL_COLOR_ATTACHMENT0 + textureIndex;
	}

	// setup depth
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.depth);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE, m_gbuffer.depth, 0);

	glDrawBuffers(GBufferTexture::noof, drawBufs);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to create GBuffer!" << std::endl;
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
*	\brief Create the LBuffer
*/
void MyView::CreateLBuffer(
	int windowWidth,
	int windowHeight
	)
{
	glGenFramebuffers(1, &m_lbuffer.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);

	glGenRenderbuffers(1, &m_lbuffer.colorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lbuffer.colorBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_lbuffer.colorBuffer);

	m_lbuffer.depth = m_gbuffer.depth;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER,0);	
}

void MyView::
windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
    assert(scene_ != nullptr);

	reloadShaders();

	{
		std::vector<glm::vec2> vertices(4);
		vertices[0] = glm::vec2(-1, -1);
		vertices[1] = glm::vec2(1, -1);
		vertices[2] = glm::vec2(1, 1);
		vertices[3] = glm::vec2(-1, 1);

		glGenBuffers(1, &m_meshQuad.getVertexVBO());
		glBindBuffer(GL_ARRAY_BUFFER, m_meshQuad.getVertexVBO());
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_meshQuad.getVAO());
		glBindVertexArray(m_meshQuad.getVAO());
		glBindBuffer(GL_ARRAY_BUFFER, m_meshQuad.getVertexVBO());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_meshQuad.Create(vertices.size(), 2);
	}

	// Load the scene
	const int noofMeshes = scene_->meshCount();
	for (int meshIndex = 0; meshIndex < noofMeshes; ++meshIndex)
	{
		const MyScene::Mesh sceneMesh = scene_->mesh(meshIndex);

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
}

void MyView::
windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
	CreateGBuffer(width, height);
	CreateLBuffer(width, height);
}

void MyView::
windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
	// free all meshes
	for (Mesh mesh : m_meshes)
		mesh.Release();

	// free all shaders
	 for( std::map<char*, Shader*>::iterator shaderIt = m_shader.begin(); shaderIt != m_shader.end(); ++shaderIt)
        delete (*shaderIt).second;
    m_shader.clear();

	glDeleteTextures(GBufferTexture::noof, m_gbuffer.texture);
	glDeleteTextures(1, &m_gbuffer.depth);

	glDeleteRenderbuffers(1, &m_lbuffer.colorBuffer);

	glDeleteFramebuffers(1, &m_gbuffer.frameBuffer);
	glDeleteFramebuffers(1, &m_lbuffer.frameBuffer);
}

void MyView::
windowViewRender(std::shared_ptr<tyga::Window> window)
{
    assert(scene_ != nullptr);

	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / static_cast<float>(viewport_size[3]);

    const MyScene::Camera camera = scene_->camera();
    
	//////////////////////////////////////////////////////////////////////////
	// POPULATE THE GBUFFER													//
	//////////////////////////////////////////////////////////////////////////

	RenderGBuffer(camera, aspect_ratio);

	//////////////////////////////////////////////////////////////////////////
	// RENDER TO THE LBUFFER FROM THE GBUFFER DATA							//
	//////////////////////////////////////////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);

	glClearColor(0.0f, 0.0f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Directional Light 
	Shader *const ambiant = m_shader["ambiant"];
	glUseProgram(ambiant->GetProgram());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[GBufferTexture::position]);
	glUniform1i(glGetUniformLocation(ambiant->GetProgram(), "sampler_world_position"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::normal]);
	glUniform1i(glGetUniformLocation(ambiant->GetProgram(), "sampler_world_normal"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::material]);
	glUniform1i(glGetUniformLocation(ambiant->GetProgram(), "sampler_material_info"), 2);

	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "directional_light_direction"), 1, glm::value_ptr(scene_->globalLightDirection()));
	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "camera_position"), 1, glm::value_ptr(camera.position));

	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());
	glBindVertexArray(0);

	// point lights
	Shader *const pointlight = m_shader["pointlight"];
	glUseProgram(pointlight->GetProgram());

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE); 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[GBufferTexture::position]);
	glUniform1i(glGetUniformLocation(pointlight->GetProgram(), "sampler_world_position"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::normal]);
	glUniform1i(glGetUniformLocation(pointlight->GetProgram(), "sampler_world_normal"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::material]);
	glUniform1i(glGetUniformLocation(pointlight->GetProgram(), "sampler_material_info"), 2);

	glUniform3fv(glGetUniformLocation(pointlight->GetProgram(), "camera_position"), 1, glm::value_ptr(camera.position));	

	glBindVertexArray(m_meshQuad.getVAO());
	for (int lightIndex = 0; lightIndex < scene_->lightCount(); ++lightIndex)
	{
		MyScene::Light light = scene_->light(lightIndex);

		glUniform1f(glGetUniformLocation(pointlight->GetProgram(), "pointlight_range"), light.range);				
		glUniform3fv(glGetUniformLocation(pointlight->GetProgram(), "pointlight_position"), 1, glm::value_ptr(light.position));	

		glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());
	}

	glDisable(GL_STENCIL_TEST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_lbuffer.frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(
		0, 0, viewport_size[2], viewport_size[3],
		0, 0, viewport_size[2], viewport_size[3],
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);	

}

/*
*	\brief Render the scene to the GBuffer
*/
void MyView::RenderGBuffer(
		 const MyScene::Camera &camera,
		 const float &aspect_ratio
	)
{
	const glm::mat4 projection_xform = glm::perspective(
		camera.vertical_field_of_view_degrees,
		aspect_ratio,
		camera.near_plane_distance,
		camera.far_plane_distance
	);

	const glm::mat4 view_xform = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		scene_->upDirection()
	);


	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer.frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);

	Shader *const gbuffer = m_shader["gbuffer"];
	glUseProgram(gbuffer->GetProgram());

	glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "viewMatrix"), 1, GL_FALSE, &view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &projection_xform[0][0]); 

	const int noofModels = scene_->modelCount();
	for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
	{
		const MyScene::Model model = scene_->model(modelIndex);
		const Mesh mesh = m_meshes[model.mesh_index];

		glm::mat4 xform = glm::mat4(model.xform);
		glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "worldMatrix"), 1, GL_FALSE, &xform[0][0]);

		const MyScene::Material mat = scene_->material(model.material_index);
		glUniform3fv(glGetUniformLocation(gbuffer->GetProgram(), "materialColor"), 1, glm::value_ptr(mat.colour));
		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "materialShininess"), mat.shininess);

		mesh.Draw();		
	}
}
