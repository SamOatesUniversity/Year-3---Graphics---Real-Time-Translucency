#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2D sampler_brick_diffuse;
uniform sampler2D sampler_brick_normal;

uniform float pointlight_range;
uniform vec3 pointlight_position;
uniform vec3 camera_position;

out vec3 reflected_light;

vec3 PointLight(vec3 worldPosition, vec3 worldNormal, vec3 lightPosition, vec3 color, float intensity, float maxrange)
{
	vec3 lightLength = lightPosition - worldPosition;
	float dist = length(lightLength);

	vec3 L = normalize(lightLength);

	float fatt = smoothstep(1.0f, 0.0f, dist / maxrange);

	float lighting = clamp(dot(L, worldNormal), 0.0f, 1.0f) * fatt;
	
	return (color * intensity) * vec3(lighting, lighting, lighting);
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

	vec3 materialColor = GetMaterialColorFromID(materialInfo.z);
	float materialShininess = materialInfo.w;

	vec3 lighting = PointLight(worldPosition, worldNormal, pointlight_position, materialColor, 1.0f, pointlight_range);

	bool usetextures = true;
	if (usetextures && (materialInfo.z > 0.05f && materialInfo.z < 0.15f))
	{
		vec3 lightLength = pointlight_position - worldPosition;
		vec3 diffuesTexture = texture(sampler_brick_diffuse, materialInfo.xy).rgb;
		vec3 normal = normalize(texture2D(sampler_brick_normal, materialInfo.xy).rgb * 2.0 - 1.0);
		vec3 L = normalize(lightLength);

		float diffuse = max(dot(normal, L), 0.0); 

		float dist = length(lightLength);
		float fatt = smoothstep(1.0f, 0.0f, dist / pointlight_range);

		diffuse = diffuse * fatt;

		reflected_light = lighting + (diffuesTexture * diffuse);
	}
	else
	{
		reflected_light = lighting;
	}
}
