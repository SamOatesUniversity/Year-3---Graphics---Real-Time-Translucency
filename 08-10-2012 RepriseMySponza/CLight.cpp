#include "CLight.h"

/*
*	\brief 
*/
CLight::CLight(
		const float coneAngle,			//!< 
		const float intensity,			//!< 
		const glm::vec3 postion,		//!< 
		const glm::vec3 direction		//!< 
	)
{
	m_light.coneAngle = coneAngle;
	m_light.intensity = intensity;
	m_light.position = postion;
	m_light.direction = direction;
}

/*
*	\brief 
*/
CLight::~CLight()
{

}

/*
*	\brief 
*/
glm::vec4 CLight::getPosition()
{
	return glm::vec4(m_light.position.x, m_light.position.y, m_light.position.z, 1.0f);
}

/*
*	\brief 
*/
glm::mat4 CLight::getView()
{
	glm::mat4 viewXform;
	viewXform = glm::lookAt(
		m_light.position,
		m_light.position + m_light.direction,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	return viewXform;
}

/*
*	\brief 
*/
glm::mat4 CLight::getProjection()
{
	glm::mat4 projectionXform;
	projectionXform = glm::perspective(
		m_light.coneAngle,
		1.0f,
		0.1f,
		200.0f
		);

	return projectionXform;
}

/*
*	\brief 
*/
void CLight::setPosition( 
		glm::vec3 &position 
	)
{
	m_light.position = position;
}

