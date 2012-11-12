#version 330

in vec3 world_position;
in vec3 world_normal;

out vec4 fragment_colour;

vec3 DirectionalLight(vec3 direction, vec3 colour, float intensity)
{
    return (colour * clamp(dot(direction, world_normal), 0.0f, 1.0f)) * intensity;
}

void main(void)
{
	vec3 light = DirectionalLight(vec3(1.0f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), 1.0f);
    fragment_colour = vec4(light, 1.0f);
}
