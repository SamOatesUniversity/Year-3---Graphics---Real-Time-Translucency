#version 330

in vec3 world_position;
in vec3 world_normal;

out vec4 fragment_colour[2];

void main(void)
{
    fragment_colour[0] = vec4(1.0f, 0.0f, 0.0f, 1.0f); //vec4(world_position, 1.0f);
	fragment_colour[1] = vec4(0.0f, 1.0f, 0.0f, 1.0f); //vec4(world_normal, 1.0f);
}
