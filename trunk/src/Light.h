#pragma once

#include "MyScene.hpp"
#include "Shader.h"
#include "Mesh.h"

enum eShadowMapSize 
{
	ShadowMapSize128,
	ShadowMapSize256,
	ShadowMapSize512,
	ShadowMapSize1024,
	ShadowMapSize2048,
	ShadowMapSize4096
};

int ShadowMapToInt();

class Light {

private:

	MyScene::Light						m_light;
	glm::mat4							m_lightxform;
	glm::mat4							m_lightview;
	glm::mat4							m_lightprojection;

public:

	bool								Enabled;
	bool								HasTranslucency;
	float								NearPlane;

public:

	static eShadowMapSize				ShadowMapSize;	
	static eShadowMapSize				OldShadowMapSize;	

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

	void								PerformLightPass(glm::vec3 sceneUp, const Shader *const shader, glm::mat4 view_xform, glm::mat4 projection_xform, glm::vec3 camera_position, const bool castShadows, const bool enableShadowPCF);

	void								Update(
											const MyScene::Light &light
										);
};