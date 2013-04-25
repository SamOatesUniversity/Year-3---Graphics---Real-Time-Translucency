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

vec3 GetIrradiance(vec2 sampleOffset)
{
	float Irradiance = texture(sampler_irradiance_map, sampleOffset).x;
	return vec3(Irradiance, Irradiance, Irradiance);
}

vec4 GetSurfaceNormal(vec2 sampleOffset)
{
	return texture(sampler_surfacenormal_map, sampleOffset);
}

vec4 GetXin(vec2 sampleOffset)
{
	return texture(sampler_worldpos_map, sampleOffset);
}

vec4 GetDepth(vec2 sampleOffset)
{
	return texture(sampler_shadow_map, sampleOffset);
}

vec3 GetXinNormal(vec4 worldPosition, vec2 sampleOffset)
{
	return texture(sampler_surfacenormal_map, sampleOffset).xyz;
}

vec3 CalculateTransformFunction(vec3 xout, vec2 sampleOffset)
{
	vec3 irradiance = GetIrradiance(sampleOffset) * 10.0f;

	const float e = 2.7182818284f;

	vec4 xin = GetXin(sampleOffset);
	vec4 surfaceNormal = GetSurfaceNormal(sampleOffset);
	vec4 depth = GetDepth(sampleOffset);

	const float sigmaScattering = 1.3f;														// os
	const float sigmaAbsorbant = 0.00000000000000001f;										// oa

	const float g = 0.1f;																	// g
	float sigmaPrimeScattering = (1 - g) * sigmaScattering;									// o's

	float sigmaExtinction = sigmaAbsorbant + sigmaScattering;								// ot
	float sigmaPrimeExtinction = sigmaAbsorbant + sigmaPrimeScattering;						// o't

	float alphaPrime = sigmaPrimeScattering / sigmaPrimeExtinction;							// &'

	const float fourPi = 12.566370614359172953850573533118f;								//4pi

	float res0 = alphaPrime / fourPi;

	///////////////////////////////////////////////////////////

	float zr = 1 / sigmaPrimeExtinction;													// zr
	float otr = sqrt(3 * sigmaAbsorbant * sigmaPrimeExtinction);							// otr
	vec3 xr = xin.xyz - dot(vec3(zr), surfaceNormal.xyz);
	float dr = length(xr - xout);															// dr

	float res1 = zr * ((otr * dr) + 1.0f) * (pow(e, -otr * dr) / (sigmaPrimeExtinction * pow(dr, 3)));

	///////////////////////////////////////////////////////////

	const float n = 1.55f;
	float Fdr = (-1.440 / pow(n, 2)) + (0.710 / n) + 0.0668 + (0.0636 * n);

	float D = 1.0f / (3.0f * sigmaPrimeExtinction);
	float A = (1 + Fdr) / (1 - Fdr);
	float zv = zr + (4 * A * D);

	vec3 xv = xin.xyz + dot(vec3(zv), surfaceNormal.xyz);
	float dv = length(xv - xout);

	float res2 = zv * ((otr * dv) + 1.0f) * (pow(e, -otr * dv) / (sigmaPrimeExtinction * pow(dv, 3)));

	///////////////////////////////////////////////////////////

	float Rd = res0 * (res1 + res2);

	return vec3(Rd * 2000000.0f);
}

vec3 CalculateTranslucentOutput(vec3 worldNormal, vec3 wout, vec3 bxOut)
{
	const float piOverOne = 0.31830988618379067153776752674503f;

	// Ft (n, win)
	const float n1 = 1.55f;
	const float n2 = 1.0f;
	float r0 = pow((n1 - n2) / (n1 + n2), 2.0f);
	float schlick = r0 + (1.0f - r0) * pow(1 - dot(wout, -worldNormal), 5.0f);

	vec3 res = piOverOne * clamp(schlick, 0.0f, 1.0f) * bxOut;

	return res;
}

vec3 SpotLight(vec4 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour, bool translucent)
{
	const float intensity = 1.0f;
	vec3 L = normalize(position - worldPosition.xyz);

    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 15.0f) * intensity;
    vec3 lighting = vec3(0);

	//translucent = false;
	if (translucent)
	{
		int count = 0;
		const int sampleSize = 0;
		const int sampleKernel = 2;

		vec2 sampleCoords = GetLightViewCoords(worldPosition);

		for (int xOffset = -sampleSize; xOffset <= sampleSize; xOffset += sampleKernel)
		{		
			for (int yOffset = -sampleSize; yOffset <= sampleSize; yOffset += sampleKernel)
			{
				vec2 sampleOffset = sampleCoords + vec2(xOffset * oneOverShadowMapSize, yOffset * oneOverShadowMapSize);
				
				vec3 xout = worldPosition.xyz;
				vec3 wout = normalize(camera_position - xout);
				vec3 bxOut = CalculateTransformFunction(xout, sampleOffset);
				vec3 Lout = CalculateTranslucentOutput(worldNormal, wout, bxOut);

				lighting += (spotLight > cos(cone)) ? colour * Lout * fatt : vec3(0.0f, 0.0f, 0.0f);

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

	vec3 lighting = SpotLight(worldPosition, worldNormal.xyz, spotlight_position, spotlight_direction, spotlight_coneangle * 1.0f, spotlight_range, materialColor, worldNormal.w >= 0.01f);

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
