#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2D sampler_shadow_map;

uniform vec3 spotlight_position;
uniform float spotlight_range;
uniform vec3 spotlight_direction;
uniform float spotlight_coneangle;

uniform mat4 light_view_xform;
uniform mat4 light_projection_xform;

uniform vec3 camera_position;

out vec3 fragment_colour;

vec3 SpotLight(vec3 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour)
{
    vec3 lightlength = position - worldPosition;
     
    vec3 N = worldNormal;
    vec3 L = normalize( lightlength );
    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 100.0f);

    vec3 lighting = (spotLight > cos(cone)) ? colour * (clamp(dot(L, N), 0.0f, 1.0f) * fatt) : vec3(0.0f, 0.0f, 0.0f);
    return lighting;
}

vec3 SpotLightPass()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec3 worldPosition = texelFetch(sampler_world_position, p).xyz;
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 lighting = SpotLight(worldPosition, worldNormal, spotlight_position, spotlight_direction, spotlight_coneangle * 0.75f, spotlight_range, vec3(1.0f, 1.0f, 1.0f));
	return lighting;
}

vec3 Shadow()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	vec4 worldPosition = texelFetch(sampler_world_position, p);

	vec4 hpos_from_light = light_projection_xform * light_view_xform * worldPosition;
    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;

	float xCoord = ((hpos_from_light.x / hpos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((hpos_from_light.y / hpos_from_light.w) + 1.0f) * 0.5f;

    vec2 shadow_texcoord = vec2(xCoord, yCoord);

	const float bias = 0.00003f;
	const int level_of_filtering = 2;
	const int kernal = 1;

	float shadowing = 0.0f;
    float count = 0.0f;
    for( int x = -level_of_filtering; x <= level_of_filtering; x += kernal )
	{
        for( int y = -level_of_filtering; y <= level_of_filtering; y += kernal )
		{
			float light_to_first_hit_depth = texture(sampler_shadow_map, shadow_texcoord + vec2(x / 2048.0f, y / 2048.0f)).x;
			shadowing += (light_to_first_hit_depth + bias) < light_to_point_depth ? 0.0f : 1.0f;
			count += 1.0f;
		}
	}

	return vec3(shadowing / count, shadowing / count, shadowing / count);
}

void main(void)
{
	vec3 spotLight = SpotLightPass();
	vec3 shadowAttenuation = Shadow();
	fragment_colour = spotLight * shadowAttenuation;
}
