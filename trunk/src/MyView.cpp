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
	m_shadowbuffer.frameBuffer = NULL;
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
	for (std::map<char*, Shader*>::iterator shaderIt = m_shader.begin(); shaderIt != m_shader.end(); ++shaderIt)
	{
		delete (*shaderIt).second;
	}
	m_shader.clear();

	ProFy::GetInstance().StartTimer(m_timer[Timer::ShaderLoadtime]);

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

	// Create a pointlight shader
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

	// Create a spotlight shader
	Shader *const spotlight = new Shader();
	if (!spotlight->Load("shaders/spotlight_vs.glsl", "shaders/spotlight_fs.glsl")) 
	{
		std::cout << "Failed to load the spotlight shader!" << std::endl;
		system("PAUSE");
	}
	else
	{
		glAttachShader(spotlight->GetProgram(), spotlight->GetVertexShader());
		glBindAttribLocation(spotlight->GetProgram(), 0, "vertex_position");

		glAttachShader(spotlight->GetProgram(), spotlight->GetFragmentShader());
		glBindFragDataLocation(spotlight->GetProgram(), 0, "fragment_colour");

		glLinkProgram(spotlight->GetProgram());

		std::cout << "Loaded spotlight shader..." << std::endl;

		m_shader["spotlight"] = spotlight;
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
		glBindAttribLocation(gbuffer->GetProgram(), 2, "vertex_tangent");
		glBindAttribLocation(gbuffer->GetProgram(), 3, "vertex_texcoord");

		glAttachShader(gbuffer->GetProgram(), gbuffer->GetFragmentShader());
		glBindFragDataLocation(gbuffer->GetProgram(), 0, "fragment_colour");

		glLinkProgram(gbuffer->GetProgram());

		std::cout << "Loaded gbuffer shader..." << std::endl;

		m_shader["gbuffer"] = gbuffer;
	}

	// Create post processing shader
	Shader *const postprocessing = new Shader();
	if (!postprocessing->Load("shaders/postprocessing_vs.glsl", "shaders/postprocessing_fs.glsl"))
	{
		std::cout << "Failed to load the post processing shader!" << std::endl;
		system("PAUSE");
	}
	else
	{
		glAttachShader(postprocessing->GetProgram(), postprocessing->GetVertexShader());
		glBindAttribLocation(postprocessing->GetProgram(), 0, "vertex_position");

		glAttachShader(postprocessing->GetProgram(), postprocessing->GetFragmentShader());
		glBindFragDataLocation(postprocessing->GetProgram(), 0, "fragment_colour");

		glLinkProgram(postprocessing->GetProgram());

		std::cout << "Loaded post processing shader..." << std::endl;

		m_shader["postprocessing"] = postprocessing;
	}

	ProFy::GetInstance().EndTimer(m_timer[Timer::ShaderLoadtime]);
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

	glGenTextures(1, &m_lbuffer.texture);
	glBindTexture(GL_TEXTURE_2D, m_lbuffer.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_lbuffer.size.x = static_cast<float>(windowWidth);
	m_lbuffer.size.y = static_cast<float>(windowHeight);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lbuffer.texture, 0);

	m_lbuffer.depth = m_gbuffer.depth;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_RECTANGLE, m_lbuffer.depth, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER,0);	
}


void MyView::CreateShadowBuffer( 
		int windowWidth, 
		int windowHeight 
	)
{
	glGenFramebuffers(1, &m_shadowbuffer.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowbuffer.frameBuffer);

	glGenTextures(1, &m_shadowbuffer.texture);
	glBindTexture(GL_TEXTURE_RECTANGLE, m_shadowbuffer.texture);

	m_shadowbuffer.size.x = static_cast<float>(windowWidth);
	m_shadowbuffer.size.y = static_cast<float>(windowHeight);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB8, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, m_shadowbuffer.texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		tglDebugMessage(GL_DEBUG_SEVERITY_HIGH, "framebuffer not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER,0);	
}


void MyView::
windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
	for (int lightIndex = 0; lightIndex < scene_->lightCount(); ++lightIndex)
	{
		m_light.push_back(new Light(scene_->light(lightIndex)));
	}

	ProFy::GetInstance().CreateTimer(m_timer[Timer::ShaderLoadtime], ProFy::TimerType::CPU, "Shader Load Time");

    assert(scene_ != nullptr);

	reloadShaders();

	// create the full screen quad
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

	// create a sphere mesh to represent the light
	{
		tsl::IndexedMesh mesh;
		tsl::CreateSphere(1.f, 12, &mesh);
		tsl::ConvertPolygonsToTriangles(&mesh);

		glGenBuffers(1, &m_sphereMesh.getVertexVBO());
		glBindBuffer(GL_ARRAY_BUFFER, m_sphereMesh.getVertexVBO());
		glBufferData(
			GL_ARRAY_BUFFER,
			mesh.vertex_array.size() * sizeof(glm::vec3),
			mesh.vertex_array.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_sphereMesh.getElementVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereMesh.getElementVBO());
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			mesh.index_array.size() * sizeof(unsigned int),
			mesh.index_array.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_sphereMesh.getVAO());
		glBindVertexArray(m_sphereMesh.getVAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereMesh.getElementVBO());
		glBindBuffer(GL_ARRAY_BUFFER, m_sphereMesh.getVertexVBO());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0, 3, 
			GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), 0
		);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_sphereMesh.Create(mesh.vertex_array.size(), mesh.index_array.size());
	}

	// create a cone to represent spotlights
	{
		tsl::IndexedMesh mesh;
		tsl::CreateCone(1.0f, 1.0f, 12, &mesh);
		tsl::ConvertPolygonsToTriangles(&mesh);

		// populate the vertex array
		const int noofVerticies = mesh.vertex_array.size();
		Vertex *const vertices = new Vertex[noofVerticies];
		for (int vertexIndex = 0; vertexIndex < noofVerticies; ++vertexIndex)
		{
			vertices[vertexIndex].position = glm::vec3(mesh.vertex_array[vertexIndex].x, mesh.vertex_array[vertexIndex].y, 1.0f - mesh.vertex_array[vertexIndex].z);
			vertices[vertexIndex].normal = glm::vec3(mesh.normal_array[vertexIndex].x * -1.0f, mesh.normal_array[vertexIndex].y * -1.0f, mesh.normal_array[vertexIndex].z * -1.0f);
		}

		// populate elements (index) array
		const unsigned int noofElements = mesh.index_array.size();
		unsigned int *const elements = new unsigned int[noofElements];
		for (unsigned int elementIndex = 0; elementIndex < noofElements; ++elementIndex) 
		{
			elements[elementIndex] = mesh.index_array[elementIndex];
		}

		// make our new mesh
		m_coneMesh.Create(vertices, noofVerticies, elements, noofElements);

		delete[] vertices;
		delete[] elements;
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
			if (sceneMesh.tangent_array.size() > 0 ) vertices[vertexIndex].tangent = sceneMesh.tangent_array[vertexIndex];
			if (sceneMesh.texcoord_array.size() > 0 ) vertices[vertexIndex].texcoord = sceneMesh.texcoord_array[vertexIndex];
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

	 tyga::Image texture_image = tyga::imageFromPNG("assets/textures/brick-dark_COLOR.png");
	 glGenTextures(1, &m_brickTexture.diffuse);
	 glBindTexture(GL_TEXTURE_2D, m_brickTexture.diffuse);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_image.width(), texture_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_image.pixels());
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 texture_image = tyga::imageFromPNG("assets/textures/brick-dark_NRM.png");
	 glGenTextures(1, &m_brickTexture.normal);
	 glBindTexture(GL_TEXTURE_2D, m_brickTexture.normal);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_image.width(), texture_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_image.pixels());
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	 glBindTexture(GL_TEXTURE_2D, 0);
}

void MyView::
windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
	CreateGBuffer(width, height);
	CreateLBuffer(width, height);
	CreateShadowBuffer(256, 256);
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

	glDeleteTextures(1, &m_lbuffer.texture);

	glDeleteFramebuffers(1, &m_gbuffer.frameBuffer);
	glDeleteFramebuffers(1, &m_lbuffer.frameBuffer);

	ProFy::GetInstance().OutputTimer(m_timer[Timer::ShaderLoadtime], true);
}

void MyView::
windowViewRender(std::shared_ptr<tyga::Window> window)
{
    assert(scene_ != nullptr);

	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / static_cast<float>(viewport_size[3]);
    const MyScene::Camera camera = scene_->camera();
    	
	// POPULATE THE GBUFFER
	RenderGBuffer(camera, aspect_ratio);

	// RENDER TO THE LBUFFER FROM THE GBUFFER DATA
	RenderLBuffer(camera, aspect_ratio);

	// POST PROCESSING
	PerformPostProcessing();
		
	// Bind to nothing
	for (unsigned int textureIndex = 0; textureIndex < 6; ++textureIndex)
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

float GetMaterialIndexFromColor(
		glm::vec3 color
	)
{
	// walls
	if (color.r == 0.8f && color.g == 0.8f && color.b == 0.8f)
		return 0.1f;

	// curtains
	if (color.r == 1.0f && color.g == 0.0f && color.b == 0.0f)
		return 0.2f;
	
	// plants
	if (color.r == 0.2f && color.g == 0.8f && color.b == 0.2f)
		return 0.3f;

	// poles
	if (color.r == 0.8f && color.g == 0.8f && color.b == 0.2f)
		return 0.4f;

	// unknown material
	return 0.0f;
}

/*
*	\brief Render the scene to the GBuffer
*/
void MyView::RenderGBuffer(
		 const MyScene::Camera &camera,									//!< The camera of which we want to render from
		 const float &aspect_ratio										//!< The aspect ratio of the window
	)
{
	// Get the projection matrix
	const glm::mat4 projection_xform = glm::perspective(
		camera.vertical_field_of_view_degrees,
		aspect_ratio,
		camera.near_plane_distance,
		camera.far_plane_distance
	);

	// get the view matrix
	const glm::mat4 view_xform = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		scene_->upDirection()
	);

	// Bind our gbuffer and clear the depth
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer.frameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(255);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	// Enable depth test to less than
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glDepthMask(GL_TRUE);

	// Set our shader to use to the gbuffer shader
	const Shader *const gbuffer = m_shader["gbuffer"];
	glUseProgram(gbuffer->GetProgram());

	// Set shader uniforms
	glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "viewMatrix"), 1, GL_FALSE, &view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &projection_xform[0][0]); 

	static int time = 0;
	time++;
	
	// Draw all our models to the gbuffer
	const int noofModels = scene_->modelCount();
	for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
	{
		const MyScene::Model model = scene_->model(modelIndex);
		const Mesh mesh = m_meshes[model.mesh_index];

		// cast our model xform to a mat4
		glm::mat4 xform = glm::mat4(model.xform);
		glUniformMatrix4fv(glGetUniformLocation(gbuffer->GetProgram(), "worldMatrix"), 1, GL_FALSE, &xform[0][0]);

		// Set the material data
		const MyScene::Material mat = scene_->material(model.material_index);
		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "materialIndex"), GetMaterialIndexFromColor(mat.colour));
		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "materialShininess"), mat.shininess);

		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "TIME"), static_cast<float>(time + modelIndex));

		// draw the mesh
		mesh.Draw();		
	}

	// disable depth testing
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Bind back to default framebuffer for safety (always use protection kids)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
*	\brief Render the LBuffer from the GBuffer data
*/
void MyView::RenderLBuffer( 
		const MyScene::Camera &camera,						//!< The camera of which we want to render from
		const float &aspect_ratio							//!< The aspect ratio of the window
	)
{
	// Bind our LBuffer and clear the color to our background colour
	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);
	glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// TODO SAM: Work out what the fuck stenciling i need to do
	glStencilFunc(GL_EQUAL, 1, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Render the directional light 
	DrawDirectionalLight(camera);

	// Draw the point lights in the scene
	//DrawPointLights(camera, aspect_ratio);

	// Draw the point lights in the scene
	DrawSpotLights(camera, aspect_ratio);

	glDisable(GL_STENCIL_TEST);
}

/*
*	\brief Perform post processing on the lbuffer
*/
void MyView::PerformPostProcessing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear the screen
	glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set our shader to use to the postprocessing shader
	const Shader *const postprocessing = m_shader["postprocessing"];
	glUseProgram(postprocessing->GetProgram());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_lbuffer.texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(postprocessing->GetProgram(), "sampler_pixel"), 0);

	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());
}

/*
*	\brief Bind all the gbuffer textures to a shader sampler
*/
void MyView::BindGBufferTextures(
		const Shader *const shader							//!< A pointer to the shader of the samplers to bind
	)
{
	// Pass in our gbuffer position texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[GBufferTexture::position]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_world_position"), 0);

	// Pass in our gbuffer normal texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::normal]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_world_normal"), 1);

	// Pass in our gbuffer material information texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::material]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_material_info"), 2);

	// Pass in our gbuffer material information texture
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE,  m_gbuffer.texture[GBufferTexture::tangents]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_world_tangent"), 3);

	// Pass in our gbuffer material information texture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,  m_brickTexture.diffuse);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_brick_diffuse"), 4);

	// Pass in our gbuffer material information texture
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D,  m_brickTexture.normal);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "sampler_brick_normal"), 5);
}

/*
*	\brief Draw the scenes directional light to the lbuffer
*/
void MyView::DrawDirectionalLight( 
		const MyScene::Camera &camera						//!< The camera of which we want to render from
	)
{
	const Shader *const ambiant = m_shader["ambiant"];
	glUseProgram(ambiant->GetProgram());

	BindGBufferTextures(ambiant);

	// Instantiate our uniforms
	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "directional_light_direction"), 1, glm::value_ptr(scene_->globalLightDirection()));
	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "camera_position"), 1, glm::value_ptr(camera.position));

	// Draw our full screen quad
	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());

	// Bind back to default for safety
	glBindVertexArray(0);
}

/*
*	\brief Draw the scenes point lights to the lbuffer
*/
void MyView::DrawPointLights( 
		const MyScene::Camera &camera,							//!< The camera of which we want to render from
		const float &aspect_ratio								//!< The aspect ratio of the window
	)
{
	// Get the projection matrix
	const glm::mat4 projection_xform = glm::perspective(
		camera.vertical_field_of_view_degrees,
		aspect_ratio,
		camera.near_plane_distance,
		camera.far_plane_distance
	);

	// get the view matrix
	const glm::mat4 view_xform = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		scene_->upDirection()
	);

	// enable blending so we don't nuke our directional light pass
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE); 

	const Shader *const pointlight = m_shader["pointlight"];
	glUseProgram(pointlight->GetProgram());

	BindGBufferTextures(pointlight);

	// Instantiate our uniforms
	glUniform3fv(glGetUniformLocation(pointlight->GetProgram(), "camera_position"), 1, glm::value_ptr(camera.position));	
	glUniformMatrix4fv(glGetUniformLocation(pointlight->GetProgram(), "viewMatrix"), 1, GL_FALSE, &view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pointlight->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &projection_xform[0][0]); 

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	for (int lightIndex = 0; lightIndex < scene_->lightCount(); ++lightIndex)
	{
		const MyScene::Light light = scene_->light(lightIndex);

		// Create a world matrix for the light mesh
		glm::mat4 xform;
		xform = glm::translate(xform, light.position);
		xform = glm::scale(xform, glm::vec3(light.range));
		glUniformMatrix4fv(glGetUniformLocation(pointlight->GetProgram(), "worldMatrix"), 1, GL_FALSE, &xform[0][0]);

		// set the current point lights data
		glUniform1f(glGetUniformLocation(pointlight->GetProgram(), "pointlight_range"), light.range);				
		glUniform3fv(glGetUniformLocation(pointlight->GetProgram(), "pointlight_position"), 1, glm::value_ptr(light.position));	

		// draw to the lbuffer
		m_sphereMesh.Draw();
	}

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);

	// Bind back to default for safety
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
*	\brief Draw the scenes spot lights to the lbuffer
*/
void MyView::DrawSpotLights( 
	const MyScene::Camera &camera,							//!< The camera of which we want to render from
	const float &aspect_ratio								//!< The aspect ratio of the window
	)
{
	// Get the projection matrix
	const glm::mat4 projection_xform = glm::perspective(
		camera.vertical_field_of_view_degrees,
		aspect_ratio,
		camera.near_plane_distance,
		camera.far_plane_distance
		);

	// get the view matrix
	const glm::mat4 view_xform = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		scene_->upDirection()
		);

	const Shader *const spotlight = m_shader["spotlight"];
	glUseProgram(spotlight->GetProgram());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//for (Light *light : m_light)
	{
		Light *light = m_light[0];
		light->CalculateWorldMatrix(scene_->upDirection());

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowbuffer.frameBuffer);
		glViewport( 0, 0, ( GLint )m_shadowbuffer.size.x, ( GLint )m_shadowbuffer.size.y );

		glClear(GL_DEPTH_BUFFER_BIT);

		BindGBufferTextures(spotlight);

		light->PerformShadowPass(scene_->upDirection(), spotlight);
		
		// draw to the lbuffer
		m_coneMesh.Draw();

		///////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);
		glViewport( 0, 0, ( GLint )m_lbuffer.size.x, ( GLint )m_lbuffer.size.y );

		BindGBufferTextures(spotlight);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_RECTANGLE, m_shadowbuffer.texture);
		glUniform1i(glGetUniformLocation(spotlight->GetProgram(), "sampler_shadow_map"), 6);

		light->PerformLightPass(scene_->upDirection(), spotlight, view_xform, projection_xform, camera.position);

		// draw to the lbuffer
		m_coneMesh.Draw();

		glDisable(GL_BLEND);
	}	

	glCullFace(GL_BACK);

	// Bind back to default for safety
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
