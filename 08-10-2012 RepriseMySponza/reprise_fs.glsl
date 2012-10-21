#version 330

// xform matracies
uniform mat4 worldMatrix;
uniform mat4 lightViewXform;
uniform mat4 lightProjectionXform;

// shadow map data
uniform float shadowMapSize;
uniform sampler2D shadowTexture;

// light data
struct Light {
	vec3	position;
	vec3	direction;
	float	coneAngle;
	float	intensity;
};

uniform Light light;

// input from vertex shader
in vec3 world_position;
in vec3 world_normal;

// output of fragmen shader
out vec4 fragment_colour;

///////////////////////////////////////////
// Create a spot light at vec3 position  //
///////////////////////////////////////////
vec3 SpotLight(vec3 position, vec3 direction, float coneAngle)
{
	vec3 world_pos = world_position;
    vec3 lightlength = position - world_pos;

	vec3 N = world_normal;
    vec3 L = normalize( lightlength );

	float spotLight = ( -dot( L, vec3(1, 0, 0) ) );

    float fatt = smoothstep( 
		cos( coneAngle * 0.5f ),
		cos( coneAngle * 0.001f ),
		spotLight 
	);

	float thing = ( clamp( dot( L, N ), 0.0, 1.0 ) * fatt );
    vec3 lighting = spotLight > 0.0f ? vec3(thing, thing, thing) : vec3( 0.0f, 0.0f, 0.0f );

    return lighting;
}

///////////////////////////////////////
// Shadow							 //
///////////////////////////////////////
vec3 Shadow(float bias, int level_of_filtering, int kernal )
{
    vec4 hpos_from_light = lightProjectionXform * lightViewXform * worldMatrix * vec4( world_position, 1.0 );

    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;

	vec2 shadow_texcoord = (hpos_from_light.xy / hpos_from_light.w) * 0.5f + 0.5f;

    float shadowing = 0.3f;
    float count = 0.0f;
    for( int x = -level_of_filtering; x <= level_of_filtering; x += kernal )
	{
        for( int y = -level_of_filtering; y <= level_of_filtering; y += kernal )
		{
            float light_to_first_hit_depth = texture(shadowTexture, shadow_texcoord + vec2( x / shadowMapSize, y / shadowMapSize )).x;
            shadowing += ((light_to_first_hit_depth+bias) < light_to_point_depth) ? 0.4f : 1.0f;
            count += 1.0f;
        }
	}

	return vec3( shadowing / count, shadowing / count, shadowing / count );
}

void main(void)
{
	vec3 colourOutput = vec3( 0.0f, 0.0f, 0.0f );

	colourOutput += SpotLight(light.position, light.direction, light.coneAngle);
	colourOutput *= Shadow(0.0000025f, 1, 1);

    fragment_colour = vec4(colourOutput.x, colourOutput.y, colourOutput.z, 1.0);
}
