#pragma once
#include "WindowControlDelegate.hpp"

class MyView;

/*
 * Tutorial: This class exists to integrate with the tyga framework. It
 *           implements the tyga::WindowControlDelegate interface which the
 *           tyga::Window object (which receives input events) uses to respond
 *           to the operating system (include mouse and keyboard actions).
 */

class MyController : public tyga::WindowControlDelegate
{
public:
	
    MyController();

    ~MyController();

private:
    /*
     * Tutorial: These methods are needed to implement the framework's
     *           tyga::WindowControlDelegate interface.
     */

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

};
