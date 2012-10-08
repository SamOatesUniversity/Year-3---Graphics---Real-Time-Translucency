#include "MyController.hpp"
#include "MyView.hpp"
#include "Window.hpp"
#include <iostream>

MyController::
	MyController()
{
	/*
	* Tutorial: Create an instance of MyView (our class to draw with OpenGL).
	*/

	view_.reset(new MyView());
}

MyController::
	~MyController()
{
}

void MyController::
	windowControlWillStart(std::shared_ptr<tyga::Window> window)
{
	/*
	* Tutorial: Assign our view object to be used by the window and perform
	*           other initialisation work now that the window exists.
	*/

	window->setView(view_);
	window->setTitle("3D Graphics Programming");
	std::cout << "Press F5 within the GL window to reload the shaders."
		<< std::endl;
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
	/*
	* Tutorial: We can use this method to respond to keyboard presses.
	*/

	if (down == false) {
		return;
	}

	switch (key_index) {
	case tyga::kWindowKeyF5:
		std::cout << "Reloading shaders ..." << std::endl;
		view_->reloadShaders();
		break;
	}
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
