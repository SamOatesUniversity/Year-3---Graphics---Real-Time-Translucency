#pragma once

#include "MyScene.hpp"
#include "Shader.h"

class Light {

private:

	MyScene::Light						m_light;
	glm::mat4							m_lightxform;

public:

										Light(
											const MyScene::Light &light
										);
										
	void								CalculateWorldMatrix(glm::vec3 sceneUp);

	void								PerformShadowPass(glm::vec3 sceneUp, const Shader *const shader);

	void								PerformLightPass(const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position);
};