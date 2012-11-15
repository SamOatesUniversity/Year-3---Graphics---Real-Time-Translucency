#include "MyController.hpp"
#include "MyView.hpp"
#include "Window.hpp"
#include <iostream>

/*
*	\brief Class constructor
*/
MyController::MyController()
{
	m_view.reset(new MyView());
}

/*
*	\brief Class destructor
*/
MyController::~MyController()
{
}

/*
*	\brief Called when the window has been created
*/
void MyController::windowControlWillStart(
		std::shared_ptr<tyga::Window> window							//!< A Pointer to the window object
	)
{
    window->setView(m_view);
    window->setTitle("Deferred Ravioli - Sam Oates 2012");
}

/*
*	\brief Called when the window is shutting down
*/
void MyController::windowControlDidStop(
		std::shared_ptr<tyga::Window> window							//!< A Pointer to the window object
	)
{
    window->setView(nullptr);
}

/*
*	\brief Called when the mouse input is moved
*/
void MyController::windowControlMouseMoved(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
        int x,															//!< The new x coord of the mouse
        int y															//!< The new y coord of the mouse
	)
{

}

/*
*	\brief Called when a mouse button state is changed
*/
void MyController::windowControlMouseButtonChanged(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
        int buttonIndex,												//!< The index of the button that was pressed
        bool down														//!< If it is a button down event or not
	)
{

}

/*
*	\brief Called when the mouse scroll wheel is changed
*/
void MyController::windowControlMouseWheelMoved(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
        int position													//!< The new position of the scroll wheel
	)
{
}

/*
*	\brief Called when a key state changes on the keyboard
*/
void MyController::windowControlKeyboardChanged(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
		int keyIndex,													//!< The index of the key that was pressed
		bool down														//!< If it is a key down event or not
	)
{
   switch (keyIndex) {
		case tyga::kWindowKeyF5:
		{
			m_view->reloadShaders();
			break;
		}

		case 'W':
		{
			m_view->getCamera()->isForwardKey = down;
			break;
		}

		case 'A':
		{
			m_view->getCamera()->isLeftKey = down;
			break;
		}

		case 'S':
		{
			m_view->getCamera()->isBackKey = down;
			break;
		}

		case 'D':
		{
			m_view->getCamera()->isRightKey = down;
			break;
		}
   }
}

/*
*	\brief Called when a thumbstick is moved on a controller
*/
void MyController::windowControlGamepadAxisMoved(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
        int gamepadIndex,												//!< The index of the controller (supports multiple contorllers)
        int axis_index,													//!< Which axis the event represents
        float pos														//!< The new position value
	)
{
}

/*
*	\brief Called when a button state changes on a controller
*/
void MyController::windowControlGamepadButtonChanged(
		std::shared_ptr<tyga::Window> window,							//!< A Pointer to the window object
        int gamepadIndex,												//!< The index of the controller (supports multiple contorllers)
        int buttonIndex,												//!< The index of the button pressed
        bool down														//!< If it is a key down event or not
	)
{
}
