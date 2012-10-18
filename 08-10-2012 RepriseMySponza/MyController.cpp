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
    window->setTitle("Real-Time Graphics");
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
	glm::vec2 mouseDif = glm::vec2(x, y) - m_lastMouse;

	m_lastMouse = glm::vec2(x, y);

	CCamera *const camera = view_->getCamera();
	if (!camera || !camera->isMouseDown)
		return;

	if (mouseDif.x != 0 || mouseDif.y != 0)
	{
		glm::vec3 turnAmount(0.0f, 0.0f, 0.0f);
		turnAmount.x = mouseDif.y * 0.1f;
		turnAmount.y = mouseDif.x * 0.1f;
		camera->Turn(turnAmount);
	}
}

void MyController::
windowControlMouseButtonChanged(std::shared_ptr<tyga::Window> window,
                                int button_index,
                                bool down)
{
	if (CCamera *const camera = view_->getCamera())
	{
		camera->isMouseDown = down;
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
		case tyga::kWindowKeyF5:
		{
			if (!down)
				return;

			std::cout << "Reloading shaders ..." << std::endl;
			view_->reloadShaders();
		}
		break;

		// handle w and up keys
		case 87:
		case tyga::kWindowKeyUp:
		{
			view_->getCamera()->isForwardKey = down;
		}
		break;

		// handle s and down keys
		case 83:
		case tyga::kWindowKeyDown:
		{
			view_->getCamera()->isBackKey = down;
		}
		break;

		// handle d and right keys
		case 68:
		case tyga::kWindowKeyRight:
		{
			view_->getCamera()->isRightKey = down;
		}
		break;

		// handle a and left keys
		case 65:
		case tyga::kWindowKeyLeft:
		{
			view_->getCamera()->isLeftKey = down;
		}
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
