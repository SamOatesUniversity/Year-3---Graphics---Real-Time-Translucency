#include "CCamera.h"

/*
* \brief Class constructor
*/
CCamera::CCamera()
{
	m_position = glm::vec3(0.0f, -100.0f, 0.0f);
	m_rotation = glm::vec3(0.0f, 270.0f, 0.0f);

	isForwardKey = false;
	isBackKey = false;
	isRightKey = false;
	isLeftKey = false;
}

/*
* \brief Class destructor
*/
CCamera::~CCamera()
{
}

/*
* \brief Get the view matrix
*/
glm::mat4 CCamera::GetViewMatrix() const
{
	glm::mat4 viewMatrix(1.0f);

	viewMatrix = glm::rotate(viewMatrix, m_rotation.x, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	viewMatrix = glm::translate(viewMatrix, m_position);	

	return viewMatrix;
}

/*
* \brief Set the cameras position
*/
void CCamera::SetPosition( 
		const glm::vec3 &position /*!< The new position of the camera */
	)
{
	m_position = position;
}

/*
* \brief Set the cameras rotation
*/
void CCamera::SetRotation( 
		const glm::vec3 &rotation /*!< The new rotation of the camera */ 
	)
{
	m_rotation = rotation;
}

/*
* \brief Move the camera from its current position by a given amount
*/
void CCamera::Move( 
		const glm::vec3 &amount /*!< The amount to move the camera */ 
	)
{
	m_position += amount;
}

/*
* \brief Turn the camera from its current rotation by a given amount
*/
void CCamera::Turn( 
		const glm::vec3 &amount /*!< The amount to turn the camera */ 
	)
{
	m_rotation += amount;
}

/*
* \brief Updates the camera position and rotation based upon key input
*/
void CCamera::Update()
{
	static const float SPEED = 1.0f;

	if (isForwardKey)
	{
		Move(glm::vec3(glm::sin(m_rotation.y * -0.017f) * SPEED, glm::sin(m_rotation.x * 0.017f) * SPEED, glm::cos(m_rotation.y * 0.017f) * SPEED));
	}
	else if (isBackKey)
	{
		Move(glm::vec3(glm::sin(m_rotation.y * -0.017f) * -SPEED, glm::sin(m_rotation.x * -0.017f) * SPEED, glm::cos(m_rotation.y * 0.017f) * -SPEED));
	}
}

