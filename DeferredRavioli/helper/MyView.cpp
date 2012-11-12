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

	Shader *ambiant = new Shader();
	if (!ambiant->Load("", "")) {
		std::cout << "Failed to load the ambiant shader!" << std::endl;
		system("PAUSE");
	}
	m_shader["ambiant"] = ambiant;
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

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	glViewport(0, 0, viewport_size[2], viewport_size[3]);
	glClearColor(0.0f, 0.f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);

	glUseProgram(m_shader["ambiant"]->GetProgram());

	glUniformMatrix4fv(glGetUniformLocation(m_shader["ambiant"]->GetProgram(), "viewMatrix"), 1, GL_FALSE, &m_camera->GetViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_shader["ambiant"]->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &m_camera->GetProjectionMatrix(aspect_ratio)[0][0]); 

	const int noofModels = m_scene->modelCount();
	for (int modelIndex = 0; modelIndex < noofModels; ++modelIndex)
	{
		const TcfScene::Model model = m_scene->model(modelIndex);
		const Mesh mesh = m_meshes[model.mesh_index];

		glUniformMatrix4fv(glGetUniformLocation(m_shader["ambiant"]->GetProgram(), "worldMatrix"), 1, GL_FALSE, &model.xform[0][0]);
		mesh.Draw();		
	}
}
