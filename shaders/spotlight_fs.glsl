#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2DRect sampler_shadow_map;

uniform sampler2D sampler_brick_diffuse;
uniform sampler2D sampler_brick_normal;

uniform float spotlight_range;
uniform vec3 spotlight_position;
uniform vec3 spotlight_direction;
uniform float spotlight_coneangle;

uniform mat4 light_view_projection_xform;

uniform int shadow_pass;
uniform vec3 camera_position;

out vec3 reflected_light;

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

vec3 GetMaterialColorFromID(float materialIndex)
{
	// curtains
	if (materialIndex > 0.15f && materialIndex < 0.25f)
		return vec3(1.0f, 0.0f, 0.0f);

	// plants
	if (materialIndex > 0.25f && materialIndex < 0.35f)
		return vec3(0.2f, 0.8f, 0.2f);

	// poles
	if (materialIndex > 0.35f && materialIndex < 0.45f)
		return vec3(0.8f, 0.8f, 0.2f);

	return vec3(0.8f, 0.8f, 0.8f);
}

vec3 SpotLightPass()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec3 worldPosition = texelFetch(sampler_world_position, p).xyz;
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 materialColor = GetMaterialColorFromID(materialInfo.z);
	float materialShininess = materialInfo.w;

	vec3 lighting = SpotLight(worldPosition, worldNormal, spotlight_position, spotlight_direction, spotlight_coneangle * 0.5f, spotlight_range, materialColor);
	return lighting;
}

vec3 Shadow()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	vec4 worldPosition = texelFetch(sampler_world_position, p);

	vec4 hpos_from_light = light_view_projection_xform * worldPosition;
    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;
    vec2 shadow_texcoord = vec2( 0.5f + 0.5f * hpos_from_light.x / hpos_from_light.w, 0.5f - 0.5f * hpos_from_light.y / hpos_from_light.w );

	int level_of_filtering = 1;
	int kernal = 1;
	int texture_size = 256;
	float bias = 0.0001f;

	float count = 0.0f;
    float shadowing = 0.0f;
    for( int x = -level_of_filtering; x <= level_of_filtering; x += kernal )
	{
        for( int y = -level_of_filtering; y <= level_of_filtering; y += kernal )
		{
			vec2 fpoint = shadow_texcoord + vec2( x / texture_size, y / texture_size );
            float light_to_first_hit_depth = texelFetch( sampler_shadow_map, ivec2(fpoint.x, fpoint.y)).x;
            shadowing += (light_to_first_hit_depth+bias) < light_to_point_depth ? 0.0f : 1.0f;
            count += 1.0f;
        }

	}

	return vec3( shadowing / count, shadowing / count, shadowing / count );
}

vec3 SpotLightShadowPass()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);
	vec4 worldPosition = texelFetch(sampler_world_position, p);

	float depth = worldPosition.z / worldPosition.w;

	return vec3(depth, 0.0f, 0.0f);
}

void main(void)
{
	if (shadow_pass == 0) {
		vec3 spotLight = SpotLightPass();
		vec3 shadowAttenuation = Shadow();
		reflected_light = spotLight * shadowAttenuation;
	} else {
		reflected_light = SpotLightShadowPass();
	}
}
