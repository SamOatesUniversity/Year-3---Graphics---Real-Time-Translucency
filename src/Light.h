#pragma once

#include "MyScene.hpp"
#include "Shader.h"
#include "Mesh.h"

#define SHADOW_MAP_SIZE 1024

#if SHADOW_MAP_SIZE == 512
	#define ONE_OVER_SHADOW_MAP_SIZE 0.001953125f
#else 
	#if SHADOW_MAP_SIZE == 1024 
		#define ONE_OVER_SHADOW_MAP_SIZE 0.0009765625f
	#else
		#if SHADOW_MAP_SIZE == 2048
			#define ONE_OVER_SHADOW_MAP_SIZE 0.00048828125f
		#else
			#if SHADOW_MAP_SIZE == 4096
				#define ONE_OVER_SHADOW_MAP_SIZE 0.000244140625f
			#endif
		#endif
	#endif
#endif

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
											const Mesh &mesh,
											const MyScene::Material &material
										);

	void								PerformLightPass(glm::vec3 sceneUp, const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position, const bool castShadows);

	void								Update(
											const MyScene::Light &light
										);
};