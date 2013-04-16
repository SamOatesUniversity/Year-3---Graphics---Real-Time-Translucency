#version 330

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float lightConeAngle;
uniform float translucency;

in vec4 lvpVertexPosition;
in vec4 lvpVertexNormal;

out vec4 fragment_colour[4];

vec4 CalculateIrradiance()
{
	vec3 Xin = lvpVertexPosition.xyz;				// Point on the surface
	vec3 Nxin = normalize(lvpVertexNormal.xyz);		// Normal vector on the surface
	vec3 Win = lightPosition - Xin;					// Incoming direction vector

	// Ft (n, win)
	float n2 = 1.55f; // MAGIC NUMBER! that represents the material of the object (kinda)
	float r0 = pow((1.0f - n2) / (1.0f + n2), 2.0f);
	float schlick = r0 + (1.0f - r0) * pow(1 - cos(dot(-Win, lightDirection)), 5.0f);

	//N(xin).win
	float LdN = dot(Nxin, Win);

	//I(win)
	float I = 1.0f;

	// Ft (n, win) | N(xin).win | I(win)
	float Exin = schlick * LdN * I;

	return vec4(Exin, 0.0f, 0.0f, 1.0f);
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

		// xin
		fragment_colour[3] =  lvpVertexPosition;
	}
}
