#include "MyController.hpp"
#include "MyView.hpp"
#include "Window.hpp"
#include <iostream>

MyController::
MyController() : camera_turn_mode_(false)
{
	view_.reset(new MyView());
}

MyController::
~MyController()
{
}

void MyController::
windowControlWillStart(std::shared_ptr<tyga::Window> window)
{
    window->setView(view_);
    window->setTitle("Real-Time Graphics :: FilterMe");
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
    static int prev_x = x;
    static int prev_y = y;
    if (camera_turn_mode_) {
        float dx = float(x - prev_x);
        float dy = float(y - prev_y);
        view_->adjustAzimuth(-dx);
        view_->adjustInclination(dy);
    }
    prev_x = x;
    prev_y = y;
}

void MyController::
windowControlMouseButtonChanged(std::shared_ptr<tyga::Window> window,
                                int button_index,
                                bool down)
{
    if (button_index == tyga::kWindowMouseButtonLeft) {
        camera_turn_mode_ = down;
    }
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
    if (down == false) {
        return;
    }

    switch (key_index) {
    case tyga::kWindowKeyF5:
        std::cout << "Reloading shaders ..." << std::endl;
        view_->reloadShaders();
        break;
    case tyga::kWindowKeyLeft:
    case 'A':
        view_->adjustInclination(1);
        break;
    case tyga::kWindowKeyRight:
    case 'D':
        view_->adjustInclination(-1);
        break;
    case tyga::kWindowKeyUp:
    case 'W':
        view_->adjustAzimuth(-1);
        break;
    case tyga::kWindowKeyDown:
    case 'S':
        view_->adjustAzimuth(1);
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
