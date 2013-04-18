#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2D sampler_shadow_map;
uniform sampler2D sampler_irradiance_map;
uniform sampler2D sampler_surfacenormal_map;
uniform sampler2D sampler_worldpos_map;

uniform vec3 spotlight_position;
uniform float spotlight_range;
uniform vec3 spotlight_direction;
uniform float spotlight_coneangle;

uniform mat4 light_view_xform;
uniform mat4 light_projection_xform;

uniform vec3 camera_position;

uniform int cast_shadows;
uniform int enableShadowPCF;
uniform float oneOverShadowMapSize;

out vec3 fragment_colour;

vec3 SpotLight(vec4 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour)
{
    vec3 lightlength = position - worldPosition.xyz;
     
    vec3 N = worldNormal;
    vec3 L = normalize( lightlength );
    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 15.0f);

	vec4 pos_from_light = light_projection_xform * light_view_xform * worldPosition;

	float xCoord = ((pos_from_light.x / pos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((pos_from_light.y / pos_from_light.w) + 1.0f) * 0.5f;
    vec2 samplePoint = vec2(xCoord, yCoord);

	vec3 xin = texture(sampler_worldpos_map, samplePoint).xyz;
	vec3 xout = worldPosition.xyz;	

	// Rd(|xi - xo|)
	float dist = length(xin - xout);

	float ldn = dot(L, N);

	// buda: 8 : 11
	// rabbit 8 : 8
	// dragon 5 : 7.5

	float lightPow = 5;
	float distExp = 7.5;

	float trans = clamp(pow(dist, lightPow) / pow(length(lightlength), distExp), 0.0f, 10.0f);
	trans = ldn <= 0.0f ? trans : 0.0f;

    vec3 lighting = (spotLight > cos(cone)) ? colour * (clamp(ldn, trans, 1.0f) * fatt) : vec3(0.0f, 0.0f, 0.0f);
    return lighting;
}

vec3 Shadow(vec4 worldPosition)
{
	vec4 hpos_from_light = light_projection_xform * light_view_xform * worldPosition;
    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;

	float xCoord = ((hpos_from_light.x / hpos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((hpos_from_light.y / hpos_from_light.w) + 1.0f) * 0.5f;

    vec2 shadow_texcoord = vec2(xCoord, yCoord);

	const float bias = 0.025f;
	int level_of_filtering = enableShadowPCF * 1;
	int kernal = 1;

	float shadowing = 0.0f;
    float count = 0.0f;
    for( int x = -level_of_filtering; x <= level_of_filtering; x += kernal )
	{
        for( int y = -level_of_filtering; y <= level_of_filtering; y += kernal )
		{
			float light_to_first_hit_depth = texture(sampler_shadow_map, shadow_texcoord + vec2(x * oneOverShadowMapSize, y * oneOverShadowMapSize)).x;
			shadowing += (light_to_first_hit_depth + bias) < light_to_point_depth ? 0.4f : 1.0f;
			count += 1.0f;
		}
	}

	return vec3(shadowing / count, shadowing / count, shadowing / count);
}

vec3 Translucency(vec4 worldPosition)
{
	vec4 pos_from_light = light_projection_xform * light_view_xform * worldPosition;

	float xCoord = ((pos_from_light.x / pos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((pos_from_light.y / pos_from_light.w) + 1.0f) * 0.5f;
    vec2 samplePoint = vec2(xCoord, yCoord);

	//E(xin) * Rd(xin, xout) * dxin

	// E(xin)
	float irradiance = texture(sampler_irradiance_map, samplePoint).x;

	vec3 surfaceNormal = texture(sampler_surfacenormal_map, samplePoint).xyz;

	vec3 xin = texture(sampler_worldpos_map, samplePoint).xyz;
	vec3 xout = worldPosition.xyz;	

	// Rd(|xi - xo|)
	float dist = length(xin - xout);

	return vec3(1, 1, 1);
}

vec3 SpotLightPass()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec4 worldPosition = texelFetch(sampler_world_position, p);
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 lighting = SpotLight(worldPosition, worldNormal, spotlight_position, spotlight_direction, spotlight_coneangle * 2.0, spotlight_range, vec3(0.3f, 0.3f, 0.3f));

	//lighting *= Translucency(worldPosition);

	if (cast_shadows == 1)
	{
		lighting *= Shadow(worldPosition); 
	}

	return lighting;
}

void main(void)
{
	fragment_colour = SpotLightPass();
}