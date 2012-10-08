#version 330

out vec4 fragment_colour;

in vec3 normal;

void main(void)
{
    fragment_colour = vec4(normal.x, normal.y, normal.z, 1.0);
}
