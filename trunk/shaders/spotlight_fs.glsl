#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;
uniform sampler2DRect sampler_world_tangent;

uniform sampler2D sampler_shadow_map;
uniform sampler2D sampler_irradiance_map;
uniform sampler2D sampler_surfacenormal_map;
uniform sampler2D sampler_worldpos_map;

uniform vec4 spotlight_color;
uniform vec3 spotlight_position;
uniform float spotlight_range;
uniform vec3 spotlight_direction;
uniform float spotlight_coneangle;
uniform float spotlight_intensity;

uniform mat4 light_view_xform;
uniform mat4 light_projection_xform;

uniform vec3 camera_position;

uniform int cast_shadows;
uniform int enableShadowPCF;
uniform float oneOverShadowMapSize;

uniform bool hasTranslucency = false;

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

vec4 CalculateTransformFunction(vec4 xout, vec2 sampleOffset)
{
	vec3 irradiance = vec3(1); // GetIrradiance(sampleOffset);

	const float e = 2.7182818284f;

	vec4 xin = GetXin(sampleOffset);
	vec4 surfaceNormal = GetSurfaceNormal(sampleOffset);
	
	const float val = 0.5f;
	const vec4 sigmaScattering = vec4(val, val, val, val);									// os
	const vec4 sigmaAbsorbant = vec4(0.0f);													// oa

	const float g = 0.0f;																	// g
	//vec4 sigmaPrimeScattering = (1 - g) * sigmaScattering;								// o's
	const vec4 sigmaPrimeScattering = sigmaScattering;
	
	//vec4 sigmaExtinction = sigmaAbsorbant + sigmaScattering;								// ot
	const vec4 sigmaExtinction = sigmaScattering;
	
	//vec4 sigmaPrimeExtinction = sigmaAbsorbant + sigmaPrimeScattering;					// o't
	const vec4 sigmaPrimeExtinction = sigmaScattering;

	//vec4 alphaPrime = sigmaPrimeScattering / sigmaPrimeExtinction;						// &'
	const vec4 alphaPrime = vec4(1.0f);
	
	const float fourPi = 12.566370614359172953850573533118f;								//4pi

	///////////////////////////////////////////////////////////

	//vec4 zr = 1 / sigmaPrimeExtinction;													// zr
	const vec4 zr = vec4(2.0f);
	
	//vec4 otr = sqrt(3 * sigmaAbsorbant * sigmaPrimeExtinction);							// otr
	const vec4 otr = vec4(0.0f);															// otr
	
	vec4 xr = xin - dot(zr, surfaceNormal);
	float dr = length(xr - xout);															// dr

	///////////////////////////////////////////////////////////

	//const float n = 1.6f;
	//float Fdr = (-1.440 / pow(n, 2)) + (0.710 / n) + 0.0668 + (0.0636 * n);
	const float Fdr = 0.04981f;

	//vec4 D = 1.0f / (3.0f * sigmaPrimeExtinction);
	const vec4 D = vec4(0.6666666666666667f);
	
	//float A = (1 + Fdr) / (1 - Fdr);
	const float A = 1.104842189456845f;
	
	//vec4 zv = zr + (4.0f * A * D);
	const vec4 zv = vec4(4.946245838551587f);

	vec4 xv = xin + dot(zv, surfaceNormal);
	float dv = length(xv - xout);

	///////////////////////////////////////////////////////////

	const float distAtten = 2.20f;

	//vec4 res1 = alphaPrime / fourPi;
	const float res1 = 0.0795774715459477f;
	
	//vec4 res2 = zr * (otr * dr + 1.0f);
	const vec4 res2 = vec4(2.0f); 
	
	//vec4 res3 = pow(vec4(e), -otr * dr) / (sigmaPrimeExtinction * pow(dr, distAtten));
	vec4 res3 = 1.0f / (sigmaPrimeExtinction * pow(dr, distAtten));
	
	//vec4 res4 = zv * (otr * dv + 1.0f);
	const vec4 res4 = vec4(4.946245838551587f);
	
	//vec4 res5 = pow(vec4(e), -otr * dv) / (sigmaPrimeExtinction * pow(dv, distAtten));
	vec4 res5 = 1.0f / (sigmaPrimeExtinction * pow(dv, distAtten));

	vec4 Rd = res1 * (res2 * res3 + res4 * res5);

	return Rd * vec4(irradiance, 0);
}

float CalculateTranslucentOutput(vec3 worldNormal, vec3 wout)
{
	
	// Ft (n, win)
	const float n1 = 1.6f;
	const float n2 = 1.0f;
	float r0 = pow((n1 - n2) / (n1 + n2), 2.0f);
	float schlick = r0 + (1.0f - r0) * pow(1 - dot(normalize(wout), -normalize(worldNormal)), 5.0f);

	return schlick;
}

vec3 SpotLight(vec4 worldPosition, vec3 worldNormal, vec3 position, vec3 direction, float cone, float maxrange, vec3 colour, bool translucent)
{
	vec3 L = normalize(position - worldPosition.xyz);

    float spotLight = dot(-L, direction);
	float fatt = smoothstep(0.0f, 1.0f, (spotLight - cos(cone * 0.5f)) * 15.0f) * spotlight_intensity;
    vec3 lighting = vec3(0);

	if (translucent && hasTranslucency)
	{
		const int sampleSize = 8;
		int sampleKernel = 4;
		int count = 0;

		vec3 xout = worldPosition.xyz;
		vec3 wout = normalize(camera_position - xout);
		const float piOverOne = 0.31830988618379067153776752674503f;
		float schlick = CalculateTranslucentOutput(worldNormal, wout);

		vec2 sampleCoords = GetLightViewCoords(worldPosition);

		for (int xOffset = -sampleSize; xOffset <= sampleSize; xOffset += sampleKernel)
		{		
			for (int yOffset = -sampleSize; yOffset <= sampleSize; yOffset += sampleKernel)
			{
				vec2 sampleOffset = sampleCoords + vec2(xOffset * oneOverShadowMapSize, yOffset * oneOverShadowMapSize);
								
				vec3 bxOut = CalculateTransformFunction(worldPosition, sampleOffset).xyz;
				vec3 Lout = clamp(piOverOne * schlick * bxOut, 0, 1);

				lighting += (spotLight > cos(cone)) ? colour * Lout * fatt : vec3(0.0f);
				count++;
			}
		}

		lighting /= count;
	}
	else
	{
		lighting = (spotLight > cos(cone)) ? colour * clamp(dot(L, worldNormal), 0, 1) * fatt : vec3(0.0f, 0.0f, 0.0f);
	}

    return lighting * spotlight_color.xyz;
}

vec3 Shadow(vec4 worldPosition, bool translucent)
{
	vec4 hpos_from_light = light_projection_xform * light_view_xform * worldPosition;
    float light_to_point_depth = hpos_from_light.z / hpos_from_light.w;

	float xCoord = ((hpos_from_light.x / hpos_from_light.w) + 1.0f) * 0.5f;
	float yCoord = ((hpos_from_light.y / hpos_from_light.w) + 1.0f) * 0.5f;

    vec2 shadow_texcoord = vec2(xCoord, yCoord);

	const float bias = 0.005f;
	const int kernal = 1;

	float shadowing = 0.0f;
    float count = 0.0f;
    for( int x = -enableShadowPCF; x <= enableShadowPCF; x += kernal )
	{
        for( int y = -enableShadowPCF; y <= enableShadowPCF; y += kernal )
		{
			float light_to_first_hit_depth = texture(sampler_shadow_map, shadow_texcoord + vec2(x * oneOverShadowMapSize, y * oneOverShadowMapSize)).x;
			shadowing += (light_to_first_hit_depth + bias) < light_to_point_depth ? translucent ? 1.0f : 0.1f : 1.0f;
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

	vec3 lighting = SpotLight(worldPosition, worldNormal.xyz, spotlight_position, spotlight_direction, spotlight_coneangle * 2.0f, spotlight_range, materialColor, worldNormal.w >= 0.01f);

	if (cast_shadows == 1)
	{
		lighting *= Shadow(worldPosition, worldNormal.w >= 0.01f); 
	}

	return lighting;
}

void main(void)
{
	fragment_colour = SpotLightPass();
}
