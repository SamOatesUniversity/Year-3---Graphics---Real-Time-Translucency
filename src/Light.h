#pragma once

#include "MyScene.hpp"
#include "Shader.h"
#include "Mesh.h"

class Light {

private:

	MyScene::Light						m_light;
	glm::mat4							m_lightxform;
	glm::mat4							m_lightview;
	glm::mat4							m_lightprojection;

public:

										Light(
											MyScene::Light &light
										);
										
	void								CalculateWorldMatrix(glm::vec3 sceneUp);

	void								PerformShadowPass(
											const Shader *const shader, 
											const MyScene::Model &model,
											const Mesh &mesh
										);

	void								PerformLightPass(glm::vec3 sceneUp, const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position);

	void								Update(
											const MyScene::Light &light
										);
};