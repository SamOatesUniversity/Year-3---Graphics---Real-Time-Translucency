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

vec2 GetLightViewCoords(vec4 worldPosition)
{
	vec4 posFromLight = light_projection_xform * light_view_xform * worldPosition;
	float xCoord = ((posFromLight.x / posFromLight.w) + 1.0f) * 0.5f;
	float yCoord = ((posFromLight.y / posFromLight.w) + 1.0f) * 0.5f;
    vec2 lightviewTexcoord = vec2(xCoord, yCoord);
	return lightviewTexcoord;
}

vec3 GetIrradiance(vec4 worldPosition, vec2 sampleOffset)
{
	vec2 lvt = GetLightViewCoords(worldPosition);
	float Irradiance = texture(sampler_irradiance_map, lvt + sampleOffset).x;
	return vec3(Irradiance, Irradiance, Irradiance);
}

vec3 GetXin(vec4 worldPosition, vec2 sampleOffset)
{
	vec2 lvt = GetLightViewCoords(worldPosition);
	return texture(sampler_worldpos_map, lvt + sampleOffset).xyz;
}

vec3 GetXinNormal(vec4 worldPosition, vec2 sampleOffset)
{
	vec2 lvt = GetLightViewCoords(worldPosition);
	return texture(sampler_surfacenormal_map, lvt + sampleOffset).xyz;
}

vec3 SpotLight(vec4 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour, bool translucent)
{
	const float intensity = 8.0f;
	vec3 L = normalize(position - worldPosition.xyz);

    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 15.0f) * intensity;
    vec3 lighting = vec3(0);

	if (translucent)
	{
		int count = 0;
		const int sampleSize = 4;
		const int sampleKernel = 2;

		for (int xOffset = -sampleSize; xOffset <= sampleSize; xOffset += sampleKernel)
		{		
			for (int yOffset = -sampleSize; yOffset <= sampleSize; yOffset += sampleKernel)
			{
				vec2 sampleOffset = vec2(xOffset * oneOverShadowMapSize, yOffset * oneOverShadowMapSize);

				vec3 irradiance = GetIrradiance(worldPosition, sampleOffset);

				vec3 xIn = GetXin(worldPosition, sampleOffset);
				vec3 xOut = worldPosition.xyz;

				vec3 xInNormal = normalize(GetXinNormal(worldPosition, sampleOffset));
				vec3 xOutNormal = normalize(worldNormal);

				vec3 xInToxOut = normalize(xOut - xIn);
				float xInxOutDistance = length(xInToxOut);
		
				vec3 trans = irradiance * dot(xInToxOut, xInNormal) * smoothstep(0.0f, 1.0f, xInxOutDistance);

				const float n1 = 1.55f;
				const float n2 = 1.0f;
				float r0 = pow((n1 - n2) / (n1 + n2), 2.0f);
				float schlick = r0 + (1.0f - r0) * pow(1 - dot(normalize(xOut-camera_position), xOutNormal), 5.0f);

				trans = clamp(schlick * trans, 0.0f, 1.0f);

				lighting += (spotLight > cos(cone)) ? colour * trans * fatt : vec3(0.0f, 0.0f, 0.0f);
				count++;
			}
		}
		lighting /= count;
	}
	else
	{
		lighting = (spotLight > cos(cone)) ? colour * clamp(dot(L, worldNormal), 0, 1) * fatt : vec3(0.0f, 0.0f, 0.0f);
	}

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
	vec4 worldNormal = texelFetch(sampler_world_normal, p);
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 materialColor = GetMaterialColorFromID(materialInfo.z);
	float materialShininess = materialInfo.w;

	vec3 lighting = SpotLight(worldPosition, worldNormal.xyz, spotlight_position, spotlight_direction, spotlight_coneangle * 0.5f, spotlight_range, materialColor, worldNormal.w >= 0.01f);

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
