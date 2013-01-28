#version 330

uniform sampler2DRect sampler_world_position;
uniform sampler2DRect sampler_world_normal;
uniform sampler2DRect sampler_material_info;

uniform float pointlight_range;
uniform vec3 pointlight_position;
uniform vec3 camera_position;

out vec3 reflected_light;

vec3 PointLight(vec3 worldPosition, vec3 worldNormal, vec3 lightPosition, vec3 color, float intensity, float maxrange)
{
	vec3 lightLength = lightPosition - worldPosition;
	float dist = length(lightLength);

	vec3 L = normalize(lightLength);

	float fatt = smoothstep(1.0f, 0.0f, dist / maxrange);

	float lighting = clamp(dot(L, worldNormal), 0.0f, 1.0f) * fatt;
	
	return (color * intensity) * vec3(lighting, lighting, lighting);
}

void main(void)
{
	ivec2 p = ivec2(gl_FragCoord.x, gl_FragCoord.y);

	vec3 worldPosition = texelFetch(sampler_world_position, p).xyz;
	vec3 worldNormal = texelFetch(sampler_world_normal, p).xyz;
	vec4 materialInfo = texelFetch(sampler_material_info, p);

	vec3 materialColor = materialInfo.xyz;
	float materialShininess = materialInfo.w;

	vec3 lighting = PointLight(worldPosition, worldNormal, pointlight_position, materialColor, 1.0f, pointlight_range);

	// specular "Shininess"... I'm pretty sure shinisness isnt a word, and if it is its not the right word, and if it is it's a wank word.
	if (materialShininess < 0.0f)
	{
		vec3 L = normalize(pointlight_position - worldPosition);
		vec3 N = worldNormal;
		vec3 V = normalize(camera_position - worldPosition);
		vec3 R = reflect(-V, N);

		float RdL = clamp(dot(R, L), 0.0, 1.0);
		vec3 specular = vec3(pow(RdL, materialShininess * 5.0f)) * 10.0f;

		lighting = lighting * specular;
	}

    reflected_light = lighting;
}
