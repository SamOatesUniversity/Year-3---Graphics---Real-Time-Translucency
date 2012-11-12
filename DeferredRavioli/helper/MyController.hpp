#pragma once
#include "WindowControlDelegate.hpp"
#include <glm/glm.hpp>

class MyView;

class MyController : public tyga::WindowControlDelegate
{
public:
	
    MyController();

    ~MyController();

private:

    void
    windowControlWillStart(std::shared_ptr<tyga::Window> window);

    void
    windowControlDidStop(std::shared_ptr<tyga::Window> window);

    void
    windowControlMouseMoved(std::shared_ptr<tyga::Window> window,
                            int x,
                            int y);	

    void
    windowControlMouseButtonChanged(std::shared_ptr<tyga::Window> window,
                                    int button_index,
                                    bool down);


    void
    windowControlMouseWheelMoved(std::shared_ptr<tyga::Window> window,
                                 int position);

    void
    windowControlKeyboardChanged(std::shared_ptr<tyga::Window> window,
                                 int key_index,
                                 bool down);

    void
    windowControlGamepadAxisMoved(std::shared_ptr<tyga::Window> window,
                                  int gamepad_index,
                                  int axis_index,
                                  float pos);

    void
    windowControlGamepadButtonChanged(std::shared_ptr<tyga::Window> window,
                                      int gamepad_index,
                                      int button_index,
                                      bool down);

private:
	std::shared_ptr<MyView> view_;

	glm::vec2				m_lastMouse;

};
