#version 330

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float translucency;

in vec4 lvpVertexPosition;
in vec4 lvpVertexNormal;

out vec4 fragment_colour[3];

vec4 CalculateIrradiance()
{
	vec3 Xin = lvpVertexNormal.xyz;
	vec3 Nxin = normalize(Xin);
	vec3 Win = normalize(Xin - lightPosition);
	float I = translucency;

	// Ft (n, win) | N(xin) win | I(win)

	float n2 = 1.55f;
	float r0 = pow((1.0f - n2) / (1.0f + n2), 2.0f);

	float schlick = r0 + (1.0f - r0) * pow(1 - cos(dot(-Win, lightDirection)), 5.0f);
	float LdN = dot(Nxin, Win);
	float part3 = translucency;

	float Exin = schlick * Ldn * part3;

	return vec4(Exin, Exin, Exin, 1.0f);
}

void main(void)
{
	// depth
	float depth = lvpVertexPosition.z / lvpVertexPosition.w;
	fragment_colour[0] =  vec4(depth, 0.0f, 0.0f, 1.0f);
	
	if (translucency != 0.0f) 
	{
		// irradiance		
		fragment_colour[1] =  CalculateIrradiance();

		// surface normal
		fragment_colour[2] =  normalize(lvpVertexNormal);
	}
}
