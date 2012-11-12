#include "MyView.hpp"
#include "FileHelper.hpp"
#include "TcfScene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/*
*	\brief Class constructor
*/
MyView::MyView()
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
}
