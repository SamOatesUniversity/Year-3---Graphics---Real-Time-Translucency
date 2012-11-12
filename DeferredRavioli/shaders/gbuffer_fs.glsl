#version 330

in vec3 world_position;
in vec3 world_normal;

out vec4 fragment_colour;

void main(void)
{
    fragment_colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
