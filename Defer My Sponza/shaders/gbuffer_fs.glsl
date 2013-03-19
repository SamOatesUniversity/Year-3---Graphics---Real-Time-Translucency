#version 330

in vec3 world_position;
in vec3 world_normal;
in vec4 material_info;

out vec4 fragment_colour[3];

void main(void)
{
    fragment_colour[0] = vec4(world_position, 1.0f);
	fragment_colour[1] = vec4(world_normal, 1.0f);
	fragment_colour[2] = material_info;
}