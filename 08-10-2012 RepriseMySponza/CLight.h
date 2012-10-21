#pragma once

#include <FileHelper.hpp>
#include <tgl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <iostream>

#define MAP_SIZE	2048

struct Light {
	glm::vec3		position;
	glm::vec3		direction;
	float			coneAngle;
	float			intensity;
};

class CLight {
private:
	Light									m_light;					//!< 
	glm::vec3								m_up;						//!< 

public:
											//!< 
											CLight(
												const float coneAngle,			//!< 
												const float intensity,			//!< 
												const glm::vec3 postion,		//!< 
												const glm::vec3 direction		//!< 
											);
	
											//!< 
											~CLight();

											//!< 
	glm::vec4								getPosition();
	
											//!< 
	glm::mat4								getView();

											//!< 
	glm::mat4								getProjection();

											//!< 
	inline Light							getLight() 
											{ 
												return m_light; 
											}

											//!< 
	void									setPosition( 
												glm::vec3 &position 
											);


	glm::vec3								getDirection()
											{
												return m_light.direction;
											}
};
