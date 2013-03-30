#include "Light.h"


Light::Light( MyScene::Light &light )
{
	m_light = light;
}

void Light::CalculateWorldMatrix(glm::vec3 sceneUp)
{
	glm::vec3 left = glm::normalize(glm::cross(m_light.direction, sceneUp));
	glm::vec3 up = glm::normalize(glm::cross(left, m_light.direction));

	glm::mat4 xform;
	xform[0][0] = left.x;				xform[0][1] = left.y;				xform[0][2] = left.z;				xform[0][3] = 0;
	xform[1][0] = up.x;					xform[1][1] = up.y;					xform[1][2] = up.z;					xform[2][3] = 0;
	xform[2][0] = m_light.direction.x;	xform[2][1] = m_light.direction.y;	xform[2][2] = m_light.direction.z;	xform[1][3] = 0;
	xform[3][0] = m_light.position.x;	xform[3][1] = m_light.position.y;	xform[3][2] = m_light.position.z;	xform[3][3] = 1;

	xform = glm::scale(xform, glm::vec3(m_light.range, m_light.range, m_light.range));

	// store the light world matrix
	m_lightxform = xform;

	// Get the projection matrix
	m_lightprojection = glm::perspective(
		m_light.field_of_view_degrees,
		1.0f,
		0.1f,
		m_light.range
	);

	// get the view matrix
	m_lightview = glm::lookAt(
		m_light.position,
		m_light.position + m_light.direction,
		sceneUp
	);
}

void Light::PerformShadowPass( 
		const Shader *const shader, 
		const MyScene::Model &model,
		const Mesh &mesh
	)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "viewMatrix"), 1, GL_FALSE, &m_lightview[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &m_lightprojection[0][0]); 

	glm::mat4 xform = glm::mat4(model.xform);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "worldMatrix"), 1, GL_FALSE, &xform[0][0]);	
}

void Light::PerformLightPass( glm::vec3 sceneUp, const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position )
{
	// Instantiate our uniforms
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "camera_position"), 1, glm::value_ptr(camera_position));	
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "viewMatrix"), 1, GL_FALSE, &view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &projection_xform[0][0]); 

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "worldMatrix"), 1, GL_FALSE, &m_lightxform[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "light_view_xform"), 1, GL_FALSE, &m_lightview[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "light_projection_xform"), 1, GL_FALSE, &m_lightprojection[0][0]);
	
	// set the current point lights data
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_range"), m_light.range);				
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_coneangle"), (m_light.field_of_view_degrees * 0.5f) * 0.017f);	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_position"), 1, glm::value_ptr(m_light.position));	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_direction"), 1, glm::value_ptr(m_light.direction));	
}

void Light::Update( const MyScene::Light &light )
{
	m_light = light;
}
