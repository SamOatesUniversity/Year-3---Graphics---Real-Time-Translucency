#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"
#include "../source/Mesh.h"

#include <glm/glm.hpp>
#include <chrono>
#include <vector>
#include <memory>

class TcfScene;

class MyView : public tyga::WindowViewDelegate
{
private:

	std::chrono::system_clock::time_point		m_startTime;			//!< 
    std::vector<Mesh>							m_meshes;				//!<
    std::shared_ptr<TcfScene>					m_scene;				//!< 

private:
												//! Called when the window starts
	void										windowViewWillStart(
													std::shared_ptr<tyga::Window> window			//!< A pointer to the window object
												);

												//! Called when the window has reset (normally caused by a resize) 
	void										windowViewDidReset(
													std::shared_ptr<tyga::Window> window,			//!< A pointer to the window object
													int width,										//!< The new width of the window
													int height										//!< The new height of the window
												);

												//! Called when the window is shutting down
	void										windowViewDidStop(
													std::shared_ptr<tyga::Window> window			//!< A pointer to the window object
												);

												//! Called when the scene should render
	void										windowViewRender(
													std::shared_ptr<tyga::Window> window			//!< A pointer to the window object
												);

public:

												//! Class constructor
												MyView();

												//! Class destructor
												~MyView();

												//! Reload the programs shaders
	void										reloadShaders();
};
