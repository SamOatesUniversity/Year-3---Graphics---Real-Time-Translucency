#version 330

in vec4 world_position;
out vec4 fragment_colour;

void main(void)
{
	float depth = world_position.z / world_position.w;
	fragment_colour = vec4(depth, depth, depth, 1.0f);
}
