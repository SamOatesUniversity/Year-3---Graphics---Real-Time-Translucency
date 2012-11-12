#include "MyController.hpp"
#include "MyView.hpp"
#include "Window.hpp"
#include <iostream>

MyController::
MyController()
{
	view_.reset(new MyView());
	m_lastMouse = glm::vec2(-1, -1);
}

MyController::
~MyController()
{
}

void MyController::
windowControlWillStart(std::shared_ptr<tyga::Window> window)
{
    window->setView(view_);
    window->setTitle("Deferred Ravioli - Sam Oates 2012");
}

void MyController::
windowControlDidStop(std::shared_ptr<tyga::Window> window)
{
    window->setView(nullptr);
}

void MyController::
windowControlMouseMoved(std::shared_ptr<tyga::Window> window,
                        int x,
                        int y)
{

}

void MyController::
windowControlMouseButtonChanged(std::shared_ptr<tyga::Window> window,
                                int button_index,
                                bool down)
{

}


void MyController::
windowControlMouseWheelMoved(std::shared_ptr<tyga::Window> window,
                             int position)
{
}

void MyController::
windowControlKeyboardChanged(std::shared_ptr<tyga::Window> window,
                             int key_index,
                             bool down)
{
   
}

void MyController::
windowControlGamepadAxisMoved(std::shared_ptr<tyga::Window> window,
                              int gamepad_index,
                              int axis_index,
                              float pos)
{
}

void MyController::
windowControlGamepadButtonChanged(std::shared_ptr<tyga::Window> window,
                                  int gamepad_index,
                                  int button_index,
                                  bool down)
{
}
