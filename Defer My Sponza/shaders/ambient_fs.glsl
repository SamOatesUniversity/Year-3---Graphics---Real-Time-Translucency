#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;

uniform vec3 directional_light_direction = vec3(0.0f, 1.0f, 0.5f);

out vec3 reflected_light;

vec3 DirectionalLight(vec3 worldNormal, vec3 direction, vec3 colour, float intensity)
{
    return colour * (clamp(dot(direction, worldNormal), 0.0, 1.0) * intensity);
}

void main(void)
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
    reflected_light = DirectionalLight(worldNormal, directional_light_direction, vec3(1, 1, 1), 1.0f);
}
