#version 330

in vec4 lvpVertexPosition;
out vec4 fragment_colour[3];

void main(void)
{
	// depth
	float depth = lvpVertexPosition.z / lvpVertexPosition.w;
	fragment_colour[0] =  vec4(depth, 0.0f, 0.0f, 1.0f);

	// irradiance
	fragment_colour[1] =  vec4(1.0f, 1.0f, 0.0f, 1.0f);

	// surface normal
	fragment_colour[2] =  vec4(0.0f, 1.0f, 1.0f, 1.0f);
}
