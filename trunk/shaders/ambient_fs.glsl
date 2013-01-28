#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2D sampler_brick_diffuse;
uniform sampler2D sampler_brick_normal;

uniform vec3 directional_light_direction = vec3(0.0f, 1.0f, 0.5f);
uniform vec3 camera_position;

out vec3 reflected_light;

vec3 DirectionalLight(vec3 worldNormal, vec3 direction, vec3 colour, float intensity)
{
    return colour * (clamp(dot(direction, worldNormal), 0.0, 1.0) * intensity);
}

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

	vec3 lighting =  DirectionalLight(worldNormal, directional_light_direction, materialColor, 1.0f);
	
	if (materialInfo.z > 0.05f && materialInfo.z < 0.15f)
	{
		vec3 diffuesTexture = texture(sampler_brick_diffuse, materialInfo.xy).rgb;
		vec3 normal = normalize(texture2D(sampler_brick_normal, materialInfo.xy).rgb * 2.0 - 1.0);
		vec3 L = normalize(directional_light_direction);
		float diffuse = max(dot(normal, L), 0.0); 

		reflected_light = lighting + (diffuesTexture * diffuse);
	}
	else
	{
		reflected_light = lighting;
	}
}
