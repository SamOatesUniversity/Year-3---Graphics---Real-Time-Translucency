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
	m_gfxCard = GFXCardType::UNKNOWN;

	m_flags.enableShadows = true;
	m_flags.enableShadowPCF = true;
	m_flags.respectShadowFlag = false;

	m_debugbar.fps = 60;
	m_debugbar.timer.gbufferCreation = 0.0f;

	m_renderbar.translucencyMode = TSM;
	
	m_lightbar.bar = nullptr;
	m_readmebar.bar = nullptr;
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
	system("cls");
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
		reloadShaders();
		return;
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

	// Create a spotlight shader
	Shader *const spotlight = new Shader();
	if (!spotlight->Load("shaders/spotlight_vs.glsl", "shaders/spotlight_fs.glsl")) 
	{
		std::cout << "Failed to load the spotlight shader!" << std::endl;
		system("PAUSE");
		reloadShaders();
		return;
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

	// Create a spotlight shader
	Shader *const spotlight_SO = new Shader();
	if (!spotlight_SO->Load("shaders/spotlight_SO_vs.glsl", "shaders/spotlight_SO_fs.glsl")) 
	{
		std::cout << "Failed to load the spotlight (SO) shader!" << std::endl;
		system("PAUSE");
		reloadShaders();
		return;
	}
	else
	{
		glAttachShader(spotlight_SO->GetProgram(), spotlight_SO->GetVertexShader());
		glBindAttribLocation(spotlight_SO->GetProgram(), 0, "vertex_position");

		glAttachShader(spotlight_SO->GetProgram(), spotlight_SO->GetFragmentShader());
		glBindFragDataLocation(spotlight_SO->GetProgram(), 0, "fragment_colour");

		glLinkProgram(spotlight_SO->GetProgram());

		std::cout << "Loaded spotlight (SO) shader..." << std::endl;

		m_shader["spotlight_so"] = spotlight_SO;
	}

	// Create a spotlight shadow shader
	Shader *const spotlightShadow = new Shader();
	if (!spotlightShadow->Load("shaders/spotlight_shadow_vs.glsl", "shaders/spotlight_shadow_fs.glsl")) 
	{
		std::cout << "Failed to load the spotlight shadow shader!" << std::endl;
		system("PAUSE");
		reloadShaders();
		return;
	}
	else
	{
		glAttachShader(spotlightShadow->GetProgram(), spotlightShadow->GetVertexShader());
		glBindAttribLocation(spotlightShadow->GetProgram(), 0, "vertex_position");
		glBindAttribLocation(spotlightShadow->GetProgram(), 1, "vertex_normal");

		glAttachShader(spotlightShadow->GetProgram(), spotlightShadow->GetFragmentShader());
		glBindFragDataLocation(spotlightShadow->GetProgram(), 0, "fragment_colour");

		glLinkProgram(spotlightShadow->GetProgram());

		std::cout << "Loaded spotlight shadow shader..." << std::endl;

		m_shader["spotlight_shadow"] = spotlightShadow;
	}

	// Create the shader which will output the world position and normal to the gbuffer
	Shader *const gbuffer = new Shader();
	if (!gbuffer->Load("shaders/gbuffer_vs.glsl", "shaders/gbuffer_fs.glsl"))
	{
		std::cout << "Failed to load the gbuffer shader!" << std::endl;
		system("PAUSE");
		reloadShaders();
		return;
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
		reloadShaders();
		return;
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer.frameBuffer);

	GLenum drawBufs[GBufferTexture::noof];

	// setup colour buffers
	for (unsigned int textureIndex = 0; textureIndex < GBufferTexture::noof; ++textureIndex) {
		glBindTexture(GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex]);
		glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureIndex, GL_TEXTURE_RECTANGLE, m_gbuffer.texture[textureIndex], 0);
		drawBufs[textureIndex] = GL_COLOR_ATTACHMENT0 + textureIndex;
	}
	glDrawBuffers(GBufferTexture::noof, drawBufs);

	// setup depth
	glBindTexture(GL_TEXTURE_2D, m_gbuffer.depth);

	if (m_gfxCard == GFXCardType::ATI) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_gbuffer.depth, 0);
	
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);
	
	glBindTexture(GL_TEXTURE_2D, m_lbuffer.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_lbuffer.size.x = static_cast<float>(windowWidth);
	m_lbuffer.size.y = static_cast<float>(windowHeight);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lbuffer.texture, 0);

	m_lbuffer.depth = m_gbuffer.depth;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_lbuffer.depth, 0);

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
	m_shadowbuffer.size.x = static_cast<float>(windowWidth);
	m_shadowbuffer.size.y = static_cast<float>(windowHeight);

	// frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowbuffer.frameBuffer);

	// render texture
	glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::depth]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::depth], 0);

	glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::irradiance]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::irradiance], 0);

	glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::surfacenormal]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::surfacenormal], 0);

	glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::xin]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_shadowbuffer.texture[TranslucencyTexture::xin], 0);
	
	GLenum drawBufs[TranslucencyTexture::noof];
	drawBufs[TranslucencyTexture::depth] = GL_COLOR_ATTACHMENT0;
	drawBufs[TranslucencyTexture::irradiance] = GL_COLOR_ATTACHMENT1;
	drawBufs[TranslucencyTexture::surfacenormal] = GL_COLOR_ATTACHMENT2;
	drawBufs[TranslucencyTexture::xin] = GL_COLOR_ATTACHMENT3;
	glDrawBuffers(TranslucencyTexture::noof, drawBufs);

	// depth stencil
	glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.depth);

	if (m_gfxCard == GFXCardType::ATI) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_shadowbuffer.depth, 0);

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
	m_light[0]->NearPlane = 10.0f;
	m_light[0]->HasTranslucency = false;
	m_light[0]->Intensity = 0.1f;

	m_light[1]->NearPlane = 10.0f;
	m_light[1]->HasTranslucency = false;
	m_light[1]->Intensity = 0.1f;

	m_light[2]->SetColor(1.0f, 0.5f, 0.5f);
	m_light[3]->SetColor(0.2f, 0.75f, 0.2f);
	m_light[4]->SetColor(0.5f, 0.2f, 0.75f);

	std::string gfxCard = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	if (gfxCard.find("ATI") != std::string::npos) {
		m_gfxCard = GFXCardType::ATI;
	} else {
		m_gfxCard = GFXCardType::NVIDIA;
	}

	ProFy::GetInstance().CreateTimer(m_timer[Timer::ShaderLoadtime], ProFy::TimerType::CPU, "Shader Load Time");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::GBufferCreation], ProFy::TimerType::OpenGL, "GBuffer Render");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_DirectionalLight], ProFy::TimerType::OpenGL, "Directional Light");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightFirstPass0], ProFy::TimerType::OpenGL, "Spot Light(0) First Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightSecondPass0], ProFy::TimerType::OpenGL, "Spot Light(0) Second Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightFirstPass1], ProFy::TimerType::OpenGL, "Spot Light(1) First Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightSecondPass1], ProFy::TimerType::OpenGL, "Spot Light(1) Second Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightFirstPass2], ProFy::TimerType::OpenGL, "Spot Light(2) First Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightSecondPass2], ProFy::TimerType::OpenGL, "Spot Light(2) Second Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightFirstPass3], ProFy::TimerType::OpenGL, "Spot Light(3) First Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightSecondPass3], ProFy::TimerType::OpenGL, "Spot Light(3) Second Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightFirstPass4], ProFy::TimerType::OpenGL, "Spot Light(4) First Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::LBuffer_SpotLightSecondPass4], ProFy::TimerType::OpenGL, "Spot Light(4) Second Pass");
	ProFy::GetInstance().CreateTimer(m_timer[Timer::PostProcessing], ProFy::TimerType::OpenGL, "Post Processing");

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
			if (!sceneMesh.tangent_array.empty()) vertices[vertexIndex].tangent = sceneMesh.tangent_array[vertexIndex];
			if (!sceneMesh.texcoord_array.empty()) vertices[vertexIndex].texcoord = sceneMesh.texcoord_array[vertexIndex];
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

	 glGenFramebuffers(1, &m_gbuffer.frameBuffer);
	 glGenTextures(GBufferTexture::noof, m_gbuffer.texture);
	 glGenTextures(1, &m_gbuffer.depth);

	 glGenFramebuffers(1, &m_lbuffer.frameBuffer);
	 glGenTextures(1, &m_lbuffer.texture);

	 glGenFramebuffers(1, &m_shadowbuffer.frameBuffer);
	 glGenTextures(TranslucencyTexture::noof, m_shadowbuffer.texture);
	 glGenTextures(1, &m_shadowbuffer.depth);

	 TwInit(TW_OPENGL_CORE, NULL);
	 CreateTweakBar();

	 // *claps ironically*
	 const_cast<MyScene*>(scene_.get())->toggleCameraAnimation();
}

void MyView::
windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
	CreateGBuffer(width, height);
	CreateLBuffer(width, height);
	CreateShadowBuffer(ShadowMapToInt(), ShadowMapToInt());
	TwWindowSize(width, height);
}

void MyView::
windowViewDidStop(std::shared_ptr<tyga::Window> window)
{
	ShowProfilingPieChart(false);
	
	TwTerminate();

	for (Light *light : m_light)
	{
		delete light;
	}
	m_light.clear();

	// free all meshes
	for (Mesh mesh : m_meshes)
		mesh.Release();

	// free all shaders
	 for( std::map<char*, Shader*>::iterator shaderIt = m_shader.begin(); shaderIt != m_shader.end(); ++shaderIt)
        delete (*shaderIt).second;
    m_shader.clear();

	for (int i = 0; i < 15; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
	}

	glDeleteTextures(GBufferTexture::noof, m_gbuffer.texture);
	glDeleteTextures(1, &m_gbuffer.depth);
	glDeleteTextures(1, &m_lbuffer.texture);
	glDeleteTextures(TranslucencyTexture::noof, m_shadowbuffer.texture);
	glDeleteTextures(1, &m_shadowbuffer.depth);

	glDeleteFramebuffers(1, &m_gbuffer.frameBuffer);
	glDeleteFramebuffers(1, &m_lbuffer.frameBuffer);
}

void MyView::
windowViewRender(std::shared_ptr<tyga::Window> window)
{
	if (Light::OldShadowMapSize != Light::ShadowMapSize)
	{
		CreateShadowBuffer(ShadowMapToInt(), ShadowMapToInt());
		Light::OldShadowMapSize = Light::ShadowMapSize;
	}

    assert(scene_ != nullptr);

	ProFy *const profy = &ProFy::GetInstance();

	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / static_cast<float>(viewport_size[3]);
    const MyScene::Camera camera = scene_->camera();
    	
	// POPULATE THE GBUFFER
	profy->StartTimer(m_timer[Timer::GBufferCreation]);
	RenderGBuffer(camera, aspect_ratio);
	profy->EndTimer(m_timer[Timer::GBufferCreation]);
	m_debugbar.timer.gbufferCreation = profy->GetTimerResult(m_timer[Timer::GBufferCreation]);

	// RENDER TO THE LBUFFER FROM THE GBUFFER DATA
	RenderLBuffer(camera, aspect_ratio);

	// POST PROCESSING
	profy->StartTimer(m_timer[Timer::PostProcessing]);
	PerformPostProcessing();
	profy->EndTimer(m_timer[Timer::PostProcessing]);
	m_debugbar.timer.postProcessing = profy->GetTimerResult(m_timer[Timer::PostProcessing]);

	m_debugbar.timer.wholeFrame = m_debugbar.timer.gbufferCreation + m_debugbar.timer.lbufferdirectional + m_debugbar.timer.lbufferspotp1 + m_debugbar.timer.lbufferspotp2 + m_debugbar.timer.postProcessing; 
	m_debugbar.fps = static_cast<int>(1000.0f / m_debugbar.timer.wholeFrame);

	// Render the debug overlay
	TwDraw();
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
	glDepthFunc(GL_LEQUAL); 

	// Enable culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
		glUniform1i(glGetUniformLocation(gbuffer->GetProgram(), "materialIndex"), model.material_index);
		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "materialShininess"), mat.shininess);
		glUniform1f(glGetUniformLocation(gbuffer->GetProgram(), "materialTrans"), mat.translucency);
		
		// draw the mesh
		mesh.Draw();		
	}

	glDisable(GL_STENCIL_TEST);

	// disable depth testing
	glDisable(GL_DEPTH_TEST);
		
	//
	glDisable(GL_CULL_FACE);

	// clear the shader to default
	glUseProgram(0);

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

	glDisable(GL_DEPTH_TEST);

	// Render the directional light 
	ProFy::GetInstance().StartTimer(m_timer[Timer::LBuffer_DirectionalLight]);
	DrawDirectionalLight(camera);
	ProFy::GetInstance().EndTimer(m_timer[Timer::LBuffer_DirectionalLight]);
	m_debugbar.timer.lbufferdirectional = ProFy::GetInstance().GetTimerResult(m_timer[Timer::LBuffer_DirectionalLight]);

	// Draw the point lights in the scene
	DrawSpotLights(camera, aspect_ratio);
}

/*
*	\brief Perform post processing on the lbuffer
*/
void MyView::PerformPostProcessing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear the screen
	glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set our shader to use to the postprocessing shader
	const Shader *const postprocessing = m_shader["postprocessing"];
	glUseProgram(postprocessing->GetProgram());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_lbuffer.texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(postprocessing->GetProgram(), "sampler_pixel"), 0);
	glUniform2fv(glGetUniformLocation(postprocessing->GetProgram(), "texcoordOffset"), 1, glm::value_ptr(glm::vec2(1.0f / m_lbuffer.size.x, 1.0f / m_lbuffer.size.y)));

	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());

	glUseProgram(0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
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

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	BindGBufferTextures(ambiant);

	// Instantiate our uniforms
	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "directional_light_direction"), 1, glm::value_ptr(scene_->globalLightDirection()));
	glUniform3fv(glGetUniformLocation(ambiant->GetProgram(), "camera_position"), 1, glm::value_ptr(camera.position));

	// Draw our full screen quad
	glBindVertexArray(m_meshQuad.getVAO());
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_meshQuad.GetNoofVertices());

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	// Bind back to default for safety
	glUseProgram(0);
	glBindVertexArray(0);
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

	const Shader *const spotlight = m_renderbar.translucencyMode == TSM ? m_shader["spotlight"] : m_shader["spotlight_so"];
	const Shader *const spotlightShadow = m_shader["spotlight_shadow"];

	ProFy *profy = &ProFy::GetInstance();

	m_debugbar.timer.lbufferspotp2 = m_debugbar.timer.lbufferspotp1 = 0.0f;
	
	static const ProFy::TimerID firstPass[] = {
		Timer::LBuffer_SpotLightFirstPass0, 
		Timer::LBuffer_SpotLightFirstPass1, 
		Timer::LBuffer_SpotLightFirstPass2, 
		Timer::LBuffer_SpotLightFirstPass3, 
		Timer::LBuffer_SpotLightFirstPass4
	};
	static const ProFy::TimerID secondPass[] = {
		Timer::LBuffer_SpotLightSecondPass0, 
		Timer::LBuffer_SpotLightSecondPass1, 
		Timer::LBuffer_SpotLightSecondPass2, 
		Timer::LBuffer_SpotLightSecondPass3, 
		Timer::LBuffer_SpotLightSecondPass4
	};

	int lightIndex = -1;
	for (Light *light : m_light)
	{
		lightIndex++;
		if (!light->Enabled)
		{
			continue;
		}

		profy->StartTimer(m_timer[firstPass[lightIndex]]);

		light->Update(scene_->light(lightIndex));
		light->CalculateWorldMatrix(scene_->upDirection());

		const bool castShadow = m_flags.enableShadows && (!m_flags.respectShadowFlag || scene_->light(lightIndex).casts_shadows);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowbuffer.frameBuffer);
		glViewport(0, 0, (GLint)m_shadowbuffer.size.x, (GLint)m_shadowbuffer.size.y);
				
		glDisable(GL_BLEND);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, ~0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); 

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glUseProgram(spotlightShadow->GetProgram());
						
		// Draw all our models to the gbuffer
		const int noofModels = scene_->modelCount();
		for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
		{
			const MyScene::Model model = scene_->model(modelIndex);
			const Mesh mesh = m_meshes[model.mesh_index];
			const MyScene::Material material = scene_->material(model.material_index);

			if (lightIndex >= 2 && m_flags.respectShadowFlag && material.translucency == 0.0f)
			{
				continue;
			}

			// buddah
			if (lightIndex == 2 && (modelIndex == 81 || modelIndex == 83))
			{
				continue;
			}

			// rabbit
			if (lightIndex == 3 && (modelIndex == 82 || modelIndex == 83))
			{
				continue;
			}

			// dragon light
			if (lightIndex == 4 && (modelIndex == 81 || modelIndex == 82))
			{
				continue;
			}

			light->PerformShadowPass(spotlightShadow, model, mesh, material);

			// draw the mesh
			mesh.Draw();		
		}

		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		profy->EndTimer(m_timer[firstPass[lightIndex]]);
		m_debugbar.timer.lbufferspotp1 += profy->GetTimerResult(m_timer[firstPass[lightIndex]]);

		///////////////////////////////////////////////////////

		profy->StartTimer(m_timer[secondPass[lightIndex]]);

		glBindFramebuffer(GL_FRAMEBUFFER, m_lbuffer.frameBuffer);
		glViewport(0, 0, (GLint)m_lbuffer.size.x, (GLint)m_lbuffer.size.y);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, ~0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS); 

		// enable blending so we don't nuke our directional light pass
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE); 

		glUseProgram(spotlight->GetProgram());

		BindGBufferTextures(spotlight);

		std::string samplerName[] = {"sampler_shadow_map", "sampler_irradiance_map", "sampler_surfacenormal_map", "sampler_worldpos_map"};

		// Pass in our translucency textures
		for (int textureIndex = 0; textureIndex < TranslucencyTexture::noof; ++textureIndex)
		{
			glActiveTexture(GL_TEXTURE4 + textureIndex);
			glBindTexture(GL_TEXTURE_2D, m_shadowbuffer.texture[textureIndex]);
			glUniform1i(glGetUniformLocation(spotlight->GetProgram(), samplerName[textureIndex].c_str()), 4 + textureIndex);
		}

		light->PerformLightPass(scene_->upDirection(), spotlight, view_xform, projection_xform, camera.position, castShadow, m_flags.enableShadowPCF);

		// draw to the lbuffer
		m_coneMesh.Draw();
		
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);

		profy->EndTimer(m_timer[secondPass[lightIndex]]);
		m_debugbar.timer.lbufferspotp2 += profy->GetTimerResult(m_timer[secondPass[lightIndex]]);
	}		
	

	// Bind back to default for safety
	glUseProgram(0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TW_CALL CallBackShowProfilingPieChart(void *data)
{
	MyView *const view = static_cast<MyView*>(data);
	view->ShowProfilingPieChart(true);
}

void TW_CALL OnCloseInfo(void *data)
{
	MyView *const view = static_cast<MyView*>(data);
	view->CloseInformation();
}

void MyView::CreateTweakBar()
{
	int opened = 0;

	// Create a bar to put all lighting settins on
	m_lightbar.bar = TwNewBar("Lighting Settings");
	TwDefine("'Lighting Settings' color='128 0 255' valueswidth=100 size='280 240' position='20 20'");  

	std::string groupName[] = {"Light One", "Light Two", "Light Three", "Light Four", "Light Five"};
	for (unsigned int lightIndex = 0; lightIndex < m_light.size(); ++lightIndex)
	{
		std::stringstream nameBuffer, paramBuffer;
		nameBuffer << "Light-" << lightIndex << "-Enabled";
		paramBuffer << "group='" << groupName[lightIndex] << "' label='Enable'";
		TwAddVarRW(m_lightbar.bar, nameBuffer.str().c_str(), TW_TYPE_BOOLCPP, &m_light[lightIndex]->Enabled, paramBuffer.str().c_str());

		nameBuffer.str(std::string()); paramBuffer.str(std::string());
		nameBuffer << "Light-" << lightIndex << "-NearClip";
		paramBuffer << "group='" << groupName[lightIndex] << "' label='Near Clip Plain'";
		TwAddVarRW(m_lightbar.bar, nameBuffer.str().c_str(), TW_TYPE_FLOAT, &m_light[lightIndex]->NearPlane, paramBuffer.str().c_str());

		nameBuffer.str(std::string()); paramBuffer.str(std::string());
		nameBuffer << "Light-" << lightIndex << "-Color";
		paramBuffer << "group='" << groupName[lightIndex] << "' label='Colour'";
		TwAddVarRW(m_lightbar.bar, nameBuffer.str().c_str(), TW_TYPE_COLOR3F, &m_light[lightIndex]->Color, paramBuffer.str().c_str());

		TwSetParam(m_lightbar.bar, groupName[lightIndex].c_str(), "opened", TW_PARAM_INT32, 1, &opened);
	}
	
	// shadow settings
	{
		TwAddVarRW (m_lightbar.bar, "ToggleShadows", TW_TYPE_BOOLCPP, &m_flags.enableShadows, "group=Shadows label='Enable Shadows'");
		TwAddVarRW (m_lightbar.bar, "TogglePCF",	 TW_TYPE_BOOLCPP, &m_flags.enableShadowPCF, "group=Shadows label='Enable PCF'");

		TwEnumVal shadowMapEV[] = { {ShadowMapSize128, "128x128"}, {ShadowMapSize256, "256x256"}, {ShadowMapSize512, "512x512"}, {ShadowMapSize1024, "1024x1024"}, {ShadowMapSize2048, "2048x2048"}, {ShadowMapSize4096, "4096x4096"} };
		TwType ShadowMapSizeEnum = TwDefineEnum("ShadowMapSizeEnum", shadowMapEV, 6);
		TwAddVarRW(m_lightbar.bar, "ShadowMapSize", ShadowMapSizeEnum, &Light::ShadowMapSize, "group=Shadows label='Texture Size'");

		TwAddVarRW (m_lightbar.bar, "ToggleRespectShadow", TW_TYPE_BOOLCPP, &m_flags.respectShadowFlag, "group=Shadows label='Respect Shadow Flag'");
	}

	// Create a bar to out put debug information...
	m_debugbar.bar = TwNewBar("Debug Information");
	TwDefine("'Debug Information' color='255 85 0' valueswidth=60 size='280 180' position='20 280' refresh=1");  
	
	TwAddVarRO (m_debugbar.bar, "FramesPerSecond", TW_TYPE_INT16, &m_debugbar.fps, "group=Generic label='Frames Per Second'");
	TwAddVarRO (m_debugbar.bar, "TimerWholeFrame", TW_TYPE_FLOAT, &m_debugbar.timer.wholeFrame, "group=Timers label='Whole Frame (ms)'");
	TwAddVarRO (m_debugbar.bar, "GBufferCreation", TW_TYPE_FLOAT, &m_debugbar.timer.gbufferCreation, "group=Timers label='Gbuffer Render (ms)'");
	TwAddVarRO (m_debugbar.bar, "LBufferDirectionalLight", TW_TYPE_FLOAT, &m_debugbar.timer.lbufferdirectional, "group=Timers label='Directional Light (ms)'");
	TwAddVarRO (m_debugbar.bar, "LBufferSpotLightP1", TW_TYPE_FLOAT, &m_debugbar.timer.lbufferspotp1, "group=Timers label='Spot Light First Pass (ms)'");
	TwAddVarRO (m_debugbar.bar, "LBufferSpotLightP2", TW_TYPE_FLOAT, &m_debugbar.timer.lbufferspotp2, "group=Timers label='Spot Light Second Pass (ms)'");
	TwAddVarRO (m_debugbar.bar, "PostProcessing", TW_TYPE_FLOAT, &m_debugbar.timer.postProcessing, "group=Timers label='PostProcessing (ms)'");

	//////////////////////////////////

	m_renderbar.bar = TwNewBar("Misc Settings");
	TwDefine("'Misc Settings' color='0 128 255' valueswidth=fit size='280 60' position='20 480'");  

	TwEnumVal translucencyModeEV[] = { {ShadowMapSize128, "Translucent Shadow Map"}, {ShadowMapSize256, "The SO Approximation"} };
	TwType translucencyModeEnum = TwDefineEnum("translucencyEnum", translucencyModeEV, 2);
	TwAddVarRW(m_renderbar.bar, "TranslucencyMode", translucencyModeEnum, &m_renderbar.translucencyMode, "label='Translucency Mode'");

	TwAddButton(m_renderbar.bar, "Show Profiling Pie Chart", CallBackShowProfilingPieChart, this, "");

	///////////////////////////////////

	m_readmebar.bar = TwNewBar("Translucent Shadow Maps by Sam Oates");
	TwDefine("'Translucent Shadow Maps by Sam Oates' valueswidth=1 size='680 300' position='320 140'"); 
	TwAddButton(m_readmebar.bar, "line0", NULL, NULL, "label='Sam Oates. University of Teesside. Student ID J9060283.'");
	TwAddButton(m_readmebar.bar, "line1", NULL, NULL, "label=' '");
	TwAddButton(m_readmebar.bar, "line2", NULL, NULL, "label='This application is a deferred renderer implementing translucent shadow maps in OpenGL 3'");
	TwAddButton(m_readmebar.bar, "line3", NULL, NULL, "label='There are five spot lights within the scene. One behind each model and two skylights.'");
	TwAddButton(m_readmebar.bar, "line4", NULL, NULL, "label=' '");
	TwAddButton(m_readmebar.bar, "line5", NULL, NULL, "label='Each light within the scene can have its color and near clip plain changed.'");
	TwAddButton(m_readmebar.bar, "line6", NULL, NULL, "label='Each light can also be disabled. This is done via the light settings bar to the top left.'");
	TwAddButton(m_readmebar.bar, "line7", NULL, NULL, "label='Using the lighting settings bar the shadows can also be modified.'");
	TwAddButton(m_readmebar.bar, "line8", NULL, NULL, "label='Within the scene data there is a flag for which lights project shadows. Enabling the'");
	TwAddButton(m_readmebar.bar, "line9", NULL, NULL, "label='\"Respect Shadow Flag\" option will enforce this behavior.'");
	TwAddButton(m_readmebar.bar, "line10", NULL, NULL, "label=' '");
	TwAddButton(m_readmebar.bar, "line11", NULL, NULL, "label='It is also worth noting that the camera will fly around the scene by default.'");
	TwAddButton(m_readmebar.bar, "line12", NULL, NULL, "label='This behavior can be toggled by pressing the \"E\" key on your keyboard.'");
	TwAddButton(m_readmebar.bar, "line13", NULL, NULL, "label=' '");
	TwAddButton(m_readmebar.bar, "line14", NULL, NULL, "label='Should the animated camera be disabled, you can explore the scene using the mouse and WASD keys.'");
	TwAddButton(m_readmebar.bar, "line15", NULL, NULL, "label=' '");
	TwAddButton(m_readmebar.bar, "line16", OnCloseInfo, this, "label='Click here to close this window'");
}

void MyView::ShowProfilingPieChart(
		bool open
	) const
{
	std::vector<ProFy::TimerID> timerIDs;
	timerIDs.push_back(m_timer[Timer::GBufferCreation]);
	timerIDs.push_back(m_timer[Timer::LBuffer_DirectionalLight]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightFirstPass0]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightSecondPass0]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightFirstPass1]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightSecondPass1]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightFirstPass2]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightSecondPass2]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightFirstPass3]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightSecondPass3]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightFirstPass4]);
	timerIDs.push_back(m_timer[Timer::LBuffer_SpotLightSecondPass4]);
	timerIDs.push_back(m_timer[Timer::PostProcessing]);
	ProFy::GetInstance().OutputTimers("AverageFrameTimes", timerIDs, GraphType::Pie, open); 
}

void MyView::CloseInformation()
{
	TwDefine("'Translucent Shadow Maps by Sam Oates' visible=false");
}
