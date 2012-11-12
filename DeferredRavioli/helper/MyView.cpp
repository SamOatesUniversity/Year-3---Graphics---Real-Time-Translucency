#include "MyView.hpp"
#include "FileHelper.hpp"
#include "TcfScene.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

MyView::MyView()
{
    scene_.reset(new TcfScene());
}

MyView::~MyView() 
{

}

void MyView::windowViewWillStart(std::shared_ptr<tyga::Window> window)
{
    start_time_ = std::chrono::system_clock::now();

    reloadShaders();

    if (!scene_->readFile("sponza.tcf")) {
        std::cerr << "Failed to read sponza.tcf data file" << std::endl;
    }
}

void MyView::reloadShaders()
{

}

void MyView::windowViewDidReset(std::shared_ptr<tyga::Window> window,
                   int width,
                   int height)
{
    glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(std::shared_ptr<tyga::Window> window)
{

}

void MyView::windowViewRender(std::shared_ptr<tyga::Window> window)
{
	GLint viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, viewport_size);
    const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	const auto clock_time = std::chrono::system_clock::now() - start_time_;
    const auto clock_millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(clock_time);
    const float time_seconds = 0.001f * clock_millisecs.count();
}
