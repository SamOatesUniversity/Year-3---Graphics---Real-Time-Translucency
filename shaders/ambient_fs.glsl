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
	// curtains
	if (materialIndex > 0.15f && materialIndex < 0.25f)
		return vec3(1.0f, 0.0f, 0.0f);

	// plants
	if (materialIndex > 0.25f && materialIndex < 0.35f)
		return vec3(0.2f, 0.8f, 0.2f);

	// poles
	if (materialIndex > 0.35f && materialIndex < 0.45f)
		return vec3(0.8f, 0.8f, 0.2f);

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
