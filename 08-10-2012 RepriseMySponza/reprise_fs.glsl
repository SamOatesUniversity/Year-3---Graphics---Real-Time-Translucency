#version 330

out vec4 fragment_colour;

in vec3 world_position;
in vec3 world_normal;

/////////////////////////////////////////////
// Create a point light at vec3 position //
/////////////////////////////////////////////
vec3 PointLight( vec3 world_position, vec3 world_normal, vec3 position, vec3 colour, float intensity, float attinuation, float maxrange )
{
	vec3 lightlength = position - world_position;
	float dist = length( lightlength );
		
	float fatt = dist < maxrange ? ( 1.0f / ( 1.0f + ( ( 0.1f / attinuation ) * ( dist * dist ) ) ) ) : 0.0f;
	float lighting = clamp( dot( normalize( lightlength ), world_normal ), 0.0, 1.0 ) * fatt;
		
	return ( colour * lighting ) * intensity;
}

void main(void)
{
	vec3 colourOutput = vec3( 0.0f, 0.0f, 0.0f );
	colourOutput += PointLight( world_position, world_normal, vec3(0.0f, 10.0f, 0.0f), vec3( 0.6f, 0.6f, 0.6f ), 0.9f, 200.0f, 2000.0f );

    fragment_colour = vec4(colourOutput.x, colourOutput.y, colourOutput.z, 1.0);
}
