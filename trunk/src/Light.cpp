#include "Light.h"


Light::Light( const MyScene::Light &light )
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

	m_lightxform = xform;
}

void Light::PerformShadowPass(glm::vec3 sceneUp, const Shader *const shader)
{
	// Get the projection matrix
	const glm::mat4 light_projection_xform = glm::perspective(
		m_light.field_of_view_degrees,
		1.0f,
		0.1f,
		m_light.range
	);

	// get the view matrix
	const glm::mat4 light_view_xform = glm::lookAt(
		m_light.position,
		m_light.position + m_light.direction,
		sceneUp
	);

	// Instantiate our uniforms
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadow_pass"), 1);	
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "viewMatrix"), 1, GL_FALSE, &light_view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &light_projection_xform[0][0]); 

	// enable blending so we don't nuke our directional light pass
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE); 

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "worldMatrix"), 1, GL_FALSE, &m_lightxform[0][0]);

	// set the current point lights data
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_range"), m_light.range);				
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_coneangle"), (m_light.field_of_view_degrees *0.5f) * 0.017f);	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_position"), 1, glm::value_ptr(m_light.position));	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_direction"), 1, glm::value_ptr(m_light.direction));	
}

void Light::PerformLightPass( glm::vec3 sceneUp, const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position )
{
	// Get the projection matrix
	const glm::mat4 light_projection_xform = glm::perspective(
		m_light.field_of_view_degrees,
		1.0f,
		0.1f,
		m_light.range
		);

	// get the view matrix
	const glm::mat4 light_view_xform = glm::lookAt(
		m_light.position,
		m_light.position + m_light.direction,
		sceneUp
	);

	const glm::mat4 light_view_projection = light_view_xform * light_projection_xform;

	// Instantiate our uniforms
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadow_pass"), 0);	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "camera_position"), 1, glm::value_ptr(camera_position));	
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "viewMatrix"), 1, GL_FALSE, &view_xform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projectionMatrix"), 1, GL_FALSE, &projection_xform[0][0]); 

	// enable blending so we don't nuke our directional light pass
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE); 

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "worldMatrix"), 1, GL_FALSE, &m_lightxform[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "light_view_projection_xform"), 1, GL_FALSE, &light_view_projection[0][0]);
	
	// set the current point lights data
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_range"), m_light.range);				
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "spotlight_coneangle"), (m_light.field_of_view_degrees *0.5f) * 0.017f);	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_position"), 1, glm::value_ptr(m_light.position));	
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "spotlight_direction"), 1, glm::value_ptr(m_light.direction));	
}
