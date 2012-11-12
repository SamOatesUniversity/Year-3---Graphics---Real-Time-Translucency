#pragma once
#include "WindowControlDelegate.hpp"
#include <glm/glm.hpp>

class MyView;

class MyController : public tyga::WindowControlDelegate
{
private:

	std::shared_ptr<MyView>					m_view;														//!< A pointer to the view object

private:
											//! Called when the window has been created
    void									windowControlWillStart(
												std::shared_ptr<tyga::Window> window					//!< A Pointer to the window object
											);

											//! Called when the window is shutting down
    void									windowControlDidStop(
												std::shared_ptr<tyga::Window> window					//!< A Pointer to the window object
											);

											//! Called when the mouse input is moved
    void									windowControlMouseMoved(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int x,													//!< The new x coord of the mouse
												int y													//!< The new y coord of the mouse
											);	

											//! Called when a mouse button state is changed
    void									windowControlMouseButtonChanged(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int buttonIndex,										//!< The index of the button that was pressed
												bool down												//!< If it is a button down event or not
											);
	
											//! Called when the mouse scroll wheel is changed
    void									windowControlMouseWheelMoved(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int position											//!< The new position of the scroll wheel
											);

											//! Called when a key state changes on the keyboard
    void									windowControlKeyboardChanged(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int keyIndex,											//!< The index of the key that was pressed
												bool down												//!< If it is a key down event or not
											);

											//! Called when a thumbstick is moved on a controller
    void									windowControlGamepadAxisMoved(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int gamepadIndex,										//!< The index of the controller (supports multiple contorllers)
												int axisIndex,											//!< Which axis the event represents
												float pos												//!< The new position value
											);

											//! Called when a button state changes on a controller
	void									windowControlGamepadButtonChanged(
												std::shared_ptr<tyga::Window> window,					//!< A Pointer to the window object
												int gamepadIndex,										//!< The index of the controller (supports multiple contorllers)
												int buttonIndex,										//!< The index of the button pressed
												bool down												//!< If it is a key down event or not
											);

public:

											//! Class constructor
											MyController();

											//! Class destructor
											~MyController();

};
