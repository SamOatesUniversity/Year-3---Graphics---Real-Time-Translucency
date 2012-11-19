#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;

uniform float pointlight_range;
uniform vec3 pointlight_position;

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

void main(void)
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec3 worldPosition = texelFetch(sampler_world_position, p).xyz;
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;

    reflected_light = PointLight(worldPosition, worldNormal, pointlight_position, vec3(1, 1, 1), 1.0f, pointlight_range);
}
