#version 330

#include "lighting/directionalLight.fs"

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform vec3 directional_light_direction = vec3(0.0f, 1.0f, 0.5f);
uniform vec3 camera_position;

out vec3 reflected_light;

vec3 GetMaterialColorFromID(float materialIndex)
{
	if (materialIndex > -0.1f && materialIndex < 0.1f)
	{
		return vec3(0.8f, 0.8f, 0.8f);
	}

	if (materialIndex > 0.9f && materialIndex < 1.1f)
	{
		return vec3(1.f, 0.33f, 0.f);
	}

	if (materialIndex > 1.9f && materialIndex < 2.1f)
	{
		return vec3(0.2f, 0.8f, 0.2f);
	}

	if (materialIndex > 2.9f && materialIndex < 3.1f)
	{
		return vec3(0.8f, 0.8f, 0.2f);
	}

	if (materialIndex > 3.9f && materialIndex < 4.1f)
	{
		return vec3(0.8f, 0.4f, 0.4f);
	}

	if (materialIndex > 4.9f && materialIndex < 5.1f)
	{
		return vec3(0.4f, 0.8f, 0.4f);
	}

	if (materialIndex > 5.9f && materialIndex < 6.1f)
	{
		return vec3(0.4f, 0.4f, 0.8f);
	}
	
	return vec3(0.8f, 0.8f, 0.8f);
}

void main(void)
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec3 worldPosition = texelFetch(sampler_world_position, p).xyz;
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);
	vec3 worldTangent = texelFetch(sampler_world_tangent, p).xyz;
	
	vec3 materialColor = GetMaterialColorFromID(materialInfo.z);
	float materialShininess = materialInfo.w;

	vec3 lighting =  DirectionalLight(worldNormal, directional_light_direction, materialColor, 0.4f);
	reflected_light = lighting;
}
