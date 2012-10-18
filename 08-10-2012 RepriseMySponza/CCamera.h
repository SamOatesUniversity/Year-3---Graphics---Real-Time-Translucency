#pragma once

#include "WindowViewDelegate.hpp"
#include "tgl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <vector>
#include <memory>

class CCamera {

private:

	glm::vec3					m_position;									//!< The current position of the camera

	glm::vec3					m_rotation;									//!< The current rotation of the camera

public:
								//! Class constructor
								CCamera();

								//! Class destructor
								~CCamera();

								//! Get the view matrix
	glm::mat4					GetViewMatrix() const;

								//! Set the cameras position
	void						SetPosition(
									const glm::vec3 &position				//!< The new position of the camera
								);

								//! Set the cameras rotation
	void						SetRotation(
									const glm::vec3 &rotation				//!< The new rotation of the camera
								);

								//! Move the camera from its current position by a given amount
	void						Move(
									const glm::vec3 &amount					//!< The amount to move the camera
								);

								//! Turn the camera from its current rotation by a given amount
	void						Turn(
									const glm::vec3 &amount					//!< The amount to turn the camera
								);

								//! Updates the camera position and rotation based upon key input
	void						Update();

public:

	bool						isForwardKey;								//!< Stores if the forward key is pressed
	bool						isBackKey;									//!< Stores if the back key is pressed
	bool						isRightKey;									//!< Stores if the right key is pressed
	bool						isLeftKey;									//!< Stores if the left key is pressed
	bool						isMouseDown;								//!< 

};