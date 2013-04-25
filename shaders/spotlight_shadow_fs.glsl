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

	vec3 lightToPoint = lightPosition - Xin;	    // Incoming direction vector
	vec3 Win = normalize(lightToPoint);				// Incoming direction vector

	// Ft (n, win)
	const float n1 = 0.8f;
	const float n2 = 5.0f; // MAGIC NUMBER! that represents the material of the object (kinda)
	float r0 = pow((n1 - n2) / (n1 + n2), 2.0f);
	float schlick = r0 + (1.0f - r0) * pow(1 - dot(Win, Nxin), 5.0f);

	//N(xin).win
	float LdN = dot(Nxin, Win);

	//I(win)
	float I = clamp(0.0f, 1.0f, 1.0f - smoothstep(0.0f, 1.0f, length(lightToPoint)));

	// Ft (n, win) | N(xin).win | I(win)
	float Exin = schlick * LdN * I;

	return vec4(Exin, 0.0f, 0.0f, 1.0f);
}

void main(void)
{
	// depth
	float depth = lvpVertexPosition.z / lvpVertexPosition.w;
	fragment_colour[0] =  vec4(depth, 0.0f, 0.0f, 1.0f);

	// irradiance		
	fragment_colour[1] =  CalculateIrradiance();

	// surface normal
	fragment_colour[2] =  normalize(lvpVertexNormal);

	// xin
	fragment_colour[3] =  lvpVertexPosition;
}
