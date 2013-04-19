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

vec3 GetMaterialColorFromID(float materialIndex)
{
	if (materialIndex > -0.1f && materialIndex < 0.1f)
	{
		return vec3(0.8f, 0.8f, 0.8f);
	}

	if (materialIndex > 0.9f && materialIndex < 1.1f)
	{
		return vec3(1.f, 0.33f, 0.f);
	}

	if (materialIndex > 1.9f && materialIndex < 2.1f)
	{
		return vec3(0.2f, 0.8f, 0.2f);
	}

	if (materialIndex > 2.9f && materialIndex < 3.1f)
	{
		return vec3(0.8f, 0.8f, 0.2f);
	}

	if (materialIndex > 3.9f && materialIndex < 4.1f)
	{
		return vec3(0.8f, 0.4f, 0.4f);
	}

	if (materialIndex > 4.9f && materialIndex < 5.1f)
	{
		return vec3(0.4f, 0.8f, 0.4f);
	}

	if (materialIndex > 5.9f && materialIndex < 6.1f)
	{
		return vec3(0.4f, 0.4f, 0.8f);
	}
	
	return vec3(0.8f, 0.8f, 0.8f);
}

float CalculateTransferFunction(vec4 worldPosition, vec3 worldNormal)
{
	vec4 posFromLight = light_projection_xform * light_view_xform * worldPosition;
	float xCoord = ((posFromLight.x / posFromLight.w) + 1.0f) * 0.5f;
	float yCoord = ((posFromLight.y / posFromLight.w) + 1.0f) * 0.5f;
    vec2 lightviewTexcoord = vec2(xCoord, yCoord);

	vec3 xIn = texture(sampler_worldpos_map, lightviewTexcoord).xyz;
	vec3 xOut = worldPosition.xyz;

	vec3 Rvec = xOut - xIn;
	vec3 surfaceNormal = texture(sampler_surfacenormal_map, lightviewTexcoord).xyz;

	float Rd = dot(Rvec, surfaceNormal);

	return pow(Rd, 1);
}

vec3 GetIrradianceAtInPoint(vec4 worldPosition)
{
	vec4 posFromLight = light_projection_xform * light_view_xform * worldPosition;
	float xCoord = ((posFromLight.x / posFromLight.w) + 1.0f) * 0.5f;
	float yCoord = ((posFromLight.y / posFromLight.w) + 1.0f) * 0.5f;
    vec2 lightviewTexcoord = vec2(xCoord, yCoord);

	float EXin = texture(sampler_irradiance_map, lightviewTexcoord).x;
	return vec3(EXin, EXin, EXin);
}

vec3 SpotLight(vec4 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour)
{
    vec3 lightlength = position - worldPosition.xyz;
    const float intensity = 0.75f;

	vec3 BXout = vec3(0, 0, 0);

	float Rd = CalculateTransferFunction(worldPosition, worldNormal);
	vec3 EXin = GetIrradianceAtInPoint(worldPosition);
	BXout = clamp(EXin * Rd, 0, 1);

	vec3 LXout = BXout;

    vec3 L = normalize( lightlength );
    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 15.0f) * intensity;
    vec3 lighting = (spotLight > cos(cone)) ? colour * LXout * fatt : vec3(0.0f, 0.0f, 0.0f);

    return lighting;
}

vec3 Shadow(vec4 worldPosition)
{
	vec4 hpos_from_light = light_projection_xform * light_view_xform * worldPosition;
    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;

	float xCoord = ((hpos_from_light.x / hpos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((hpos_from_light.y / hpos_from_light.w) + 1.0f) * 0.5f;

    vec2 shadow_texcoord = vec2(xCoord, yCoord);

	const float bias = 0.0025f;
	const int kernal = 1;

	float shadowing = 0.0f;
    float count = 0.0f;
    for( int x = -enableShadowPCF; x <= enableShadowPCF; x += kernal )
	{
        for( int y = -enableShadowPCF; y <= enableShadowPCF; y += kernal )
		{
			float light_to_first_hit_depth = texture(sampler_shadow_map, shadow_texcoord + vec2(x * oneOverShadowMapSize, y * oneOverShadowMapSize)).x;
			shadowing += (light_to_first_hit_depth + bias) < light_to_point_depth ? 0.2f : 1.0f;
			count++;
		}
	}

	return vec3(shadowing / count, shadowing / count, shadowing / count);
}

vec3 SpotLightPass()
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec4 worldPosition = texelFetch(sampler_world_position, p);
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 materialColor = GetMaterialColorFromID(materialInfo.z);
	float materialShininess = materialInfo.w;

	vec3 lighting = SpotLight(worldPosition, worldNormal, spotlight_position, spotlight_direction, spotlight_coneangle * 2.0, spotlight_range, materialColor);

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
