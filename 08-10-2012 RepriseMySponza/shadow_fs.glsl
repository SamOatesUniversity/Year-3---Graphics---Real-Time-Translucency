#version 330

in vec4 vertexPos;

out vec4 fragment_colour;

void main(void)
{
	float depth = vertexPos.z / vertexPos.w;
	fragment_colour = vec4(depth, 0, 0, 1);
}
