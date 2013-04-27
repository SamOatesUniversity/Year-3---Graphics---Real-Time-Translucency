#include "MyController.hpp"
#include "MyView.hpp"
#include "MyScene.hpp"
#include "Window.hpp"
#include <iostream>

MyController::
MyController() : camera_turn_mode_(false)
{
    camera_move_key_[0] = false;
    camera_move_key_[1] = false;
    camera_move_key_[2] = false;
    camera_move_key_[3] = false;
    scene_.reset(new MyScene());
	view_.reset(new MyView());
    view_->setScene(scene_);
}

MyController::
~MyController()
{
}

void MyController::
windowControlWillStart(std::shared_ptr<tyga::Window> window)
{
    window->setView(view_);
    window->setTitle("Real-Time Graphics :: DeferMySponza");
}

void MyController::
windowControlDidStop(std::shared_ptr<tyga::Window> window)
{
    window->setView(nullptr);
}

void MyController::
windowControlViewWillRender(std::shared_ptr<tyga::Window> window)
{
    scene_->update();
    //if (camera_turn_mode_) {
        scene_->setCameraRotationSpeed(0, 0);
    //}
}

void MyController::
windowControlMouseMoved(std::shared_ptr<tyga::Window> window,
                        int x,
                        int y)
{
    static int prev_x = x;
    static int prev_y = y;
    if (camera_turn_mode_) {
        int dx = x - prev_x;
        int dy = y - prev_y;
        const float mouse_speed = 0.6f;
        scene_->setCameraRotationSpeed(-dx * mouse_speed, -dy * mouse_speed);
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
    switch (key_index) {
    case tyga::kWindowKeyLeft:
    case 'A':
        camera_move_key_[0] = down;
        break;
    case tyga::kWindowKeyRight:
    case 'D':
        camera_move_key_[1] = down;
        break;
    case tyga::kWindowKeyUp:
    case 'W':
        camera_move_key_[2] = down;
        break;
    case tyga::kWindowKeyDown:
    case 'S':
        camera_move_key_[3] = down;
        break;
	case 262:
		if (down) {
			view_->reloadShaders();
		}
		break;
	case 'E':
		if (down) {
			scene_->toggleCameraAnimation();
		}
		break;
    }

    const float key_speed = 100.f;
    const float sideward_speed = -key_speed * camera_move_key_[0]
                                    + key_speed * camera_move_key_[1];
    const float forward_speed = key_speed * camera_move_key_[2]
                                - key_speed * camera_move_key_[3];
    scene_->setCameraTranslationSpeed(sideward_speed, forward_speed);
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
