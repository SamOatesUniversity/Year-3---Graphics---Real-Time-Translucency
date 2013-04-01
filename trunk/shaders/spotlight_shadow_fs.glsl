#version 330

uniform vec3 lightPosition;
uniform float translucency;

in vec4 lvpVertexPosition;
in vec4 lvpVertexNormal;

out vec4 fragment_colour[3];

void main(void)
{
	// depth
	float depth = lvpVertexPosition.z / lvpVertexPosition.w;
	fragment_colour[0] =  vec4(depth, 0.0f, 0.0f, 1.0f);
	
	if (translucency != 0.0f) 
	{
		// irradiance
		vec3 N = normalize(lvpVertexNormal.xyz);
		vec3 L = normalize(lvpVertexPosition.xyz - lightPosition);

		float k = pow(1.0f - max(0.0f, dot(N, L)), 5.0f);
		float Ft = 1.0f - k + 1.0f * k;

		float intensity = 1.0f + translucency;
		vec3 E = vec3(Ft * max(0.0f, dot(N, L))) * intensity;

		fragment_colour[1] =  vec4(E, 1.0f);

		// surface normal
		fragment_colour[2] =  lvpVertexNormal;
	}
}
