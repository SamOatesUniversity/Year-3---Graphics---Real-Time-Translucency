/**
 @file    Window.cpp
 @author  Tyrone Davison
 @date    September 2012
 */

#include "Window.hpp"
#include "WindowViewDelegate.hpp"
#include "WindowControlDelegate.hpp"
#include "tgl.h"
#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <GL/glfw.h>

namespace tyga
{

	bool Window::m_closed = false;

std::shared_ptr<Window> Window::main_window_;

Window::GamepadState Window::gamepad_state_[Window::MAX_GAMEPADS];

std::shared_ptr<Window> Window::
mainWindow()
{
    if (main_window_ == nullptr) {
        main_window_.reset(new Window());
    }
    return main_window_;
}

std::shared_ptr<WindowViewDelegate> Window::
view() const
{
    return view_;
}

void Window::
setView(std::shared_ptr<WindowViewDelegate> view)
{
	if (view == view_) {
		return;
    }
    if (isVisible()) {
	    if (view_ != nullptr) {
		    view_->windowViewDidStop(shared_from_this());
	    }
        view_ = view;
	    if (view_ != nullptr) {
    	    view_->windowViewWillStart(shared_from_this());
            glfwSetWindowSizeCallback(onResize); // to force resize event
	    }
    } else {
        view_ = view;
    }
}

std::shared_ptr<WindowControlDelegate> Window::
controller() const
{
    return controller_;
}

void Window::
setController(std::shared_ptr<WindowControlDelegate> controller)
{
    if (controller == controller_) {
        return;
    }
    if (isVisible()) {
        if (controller_ != nullptr) {
            controller_->windowControlDidStop(shared_from_this());
        }
        controller_ = controller;
	    if (controller_ != nullptr) {
            controller_->windowControlWillStart(shared_from_this());
	    }
    } else {
        controller_ = controller;
    }
}

bool Window::
open(int width,
     int height,
     int sample_count,
     bool windowed)
{
    if (glfwInit() == 0) {
        return false;
    }

    const int major_version = 3;
    const int minor_version = 3;

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, sample_count);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, major_version);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, minor_version);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    const int open_mode = windowed ? GLFW_WINDOW : GLFW_FULLSCREEN;
    if (glfwOpenWindow(width, height, 8, 8, 8, 0, 24, 8, open_mode) == 0) {
        glfwTerminate();
        return false;
    }

    tglInit();
    if (tglIsAvailable(TGL_EXTENSION_GL_3_3) != GL_TRUE) {
        glfwTerminate();
        return false;
    }

    if (view_ != nullptr) {
    	view_->windowViewWillStart(shared_from_this());
    }
    
    glfwSetWindowSizeCallback(onResize);
    glfwSetWindowCloseCallback(onClose);
    glfwSetMousePosCallback(onMouseMove);
    glfwSetMouseWheelCallback(onMouseWheel);
    glfwSetMouseButtonCallback(onMouseButton);
    glfwSetKeyCallback(onKeyEvent);

    if (controller_ != nullptr) {
        controller_->windowControlWillStart(shared_from_this());
    }

	m_closed = false;

    return true;
}

bool Window::
isVisible() const
{
    return glfwGetWindowParam(GLFW_OPENED) != 0;
}

void Window::
update()
{
    if (isVisible() == false) {
        return;
    }
    if (view_ != nullptr) {
        if (controller_ != nullptr) {
            controller_->windowControlViewWillRender(shared_from_this());
        }
        view_->windowViewRender(shared_from_this());
    } else {
        glClearColor(0.25f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

	glfwSwapBuffers();

	if (!m_closed) {
		pollGamepads();
	}
}

void Window::
close()
{
    glfwCloseWindow();
    glfwTerminate();
    if (main_window_.get() == this) {
        main_window_.reset();
    }
}

void Window::
setTitle(std::string newTitle)
{
    glfwSetWindowTitle(newTitle.c_str());
}

void Window::
resize(int width,
       int height)
{
    glfwSetWindowSize(width, height);
}

void Window::
setFullscreen(bool yes)
{
    if (yes) {
        glfwRestoreWindow();
    } else {
        glfwIconifyWindow();
    }
}

void Window::
onResize(int width,
         int height)
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->view_ != nullptr) {
        window->view_->windowViewDidReset(window->shared_from_this(),
                                          width, height);
    }
}

int Window::
onClose()
{
	m_closed = true;
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr) {
        if (window->controller_ != nullptr) {
            window->controller_
              ->windowControlDidStop(window->shared_from_this());
        }
        if (window->view_ != nullptr) {
            window->view_->windowViewDidStop(window->shared_from_this());
        }
    }
    return GL_TRUE;
}

void Window::
onMouseMove(int x,
            int y)
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_->windowControlMouseMoved(window->shared_from_this(),
                                                     x,
                                                     y);
    }
}

void Window::
onMouseWheel(int pos)
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlMouseWheelMoved(window->shared_from_this(),
                                         pos);
    }
}

void Window::
onMouseButton(int button,
              int action)
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlMouseButtonChanged(window->shared_from_this(),
                                            button,
                                            action == GLFW_PRESS);
    }
}

void Window::
onKeyEvent(int key,
           int action)
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        window->controller_
          ->windowControlKeyboardChanged(window->shared_from_this(),
                                         key,
                                         action == GLFW_PRESS);
    }
}

void Window::
pollGamepads()
{
    // NB: GLFW limitation limits operation to a single window
    Window* window = main_window_.get();
    if (window != nullptr && window->controller_ != nullptr) {
        for (int i=0; i<MAX_GAMEPADS; ++i) {
            if (glfwGetJoystickParam(i, GLFW_PRESENT) == GL_FALSE) {
                continue;
            }
            GamepadState newState;
            glfwGetJoystickPos(i, newState.axis, MAX_GAMEPAD_AXES);
            glfwGetJoystickButtons(i, newState.button, MAX_GAMEPAD_BUTTONS);
            // a few hacks to standardize results from a Microsoft Gamepad
            newState.axis[3] = -newState.axis[3];
            newState.axis[7] = newState.axis[2] < 0.f ? -newState.axis[2] : 0.f;
            newState.axis[2] = newState.axis[2] > 0.f ? newState.axis[2] : 0.f;
            // TODO: dpad seems to be missing
            for (int j=0; j<MAX_GAMEPAD_AXES; ++j) {
                if (newState.axis[j] != gamepad_state_[i].axis[j]) {
                    window->controller_->
                      windowControlGamepadAxisMoved(window->shared_from_this(),
                                                    i,
                                                    j,
                                                    newState.axis[j]);
                }
            }
            for (int j=0; j<MAX_GAMEPAD_BUTTONS; ++j) {
                if (newState.button[j] != gamepad_state_[i].button[j]) {
                    window->controller_->windowControlGamepadButtonChanged(
                                            window->shared_from_this(),
                                            i,
                                            j,
                                            newState.button[j] == GLFW_PRESS);
                }
            }
            gamepad_state_[i] = newState;
        }
    }
}

Window::
Window()
{
}

Window::
~Window()
{
}

Window::GamepadState::
GamepadState()
{
    for (int i=0; i<MAX_GAMEPAD_AXES; ++i) {
        axis[i] = 0.f;
    }
    for (int i=0; i<MAX_GAMEPAD_BUTTONS; ++i) {
        button[i] = 0;
    }
}

} // end namespace tyga
