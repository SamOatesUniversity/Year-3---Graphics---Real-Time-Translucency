#version 330

in vec4 lvpVertexPosition;
out vec4 fragment_colour;

void main(void)
{
	float depth = lvpVertexPosition.z / lvpVertexPosition.w;
	fragment_colour =  vec4(depth, 0.0f, 0.0f, 1.0f);
}
