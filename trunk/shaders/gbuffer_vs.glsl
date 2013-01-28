#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

uniform float materialIndex;
uniform float materialShininess; 

uniform float TIME;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;

out vec3 world_position;
out vec3 world_normal;
out vec3 world_tangent;
out vec4 material_info;

void main(void)
{
	vec3 vpos = vertex_position;
	if (materialIndex > 0.15f && materialIndex < 0.25f)
	{
		vpos.x = vpos.x - (cos(TIME * 0.0025f) * 0.5f);
		vpos.z = vpos.z - (((sin(TIME * 0.01f) * 0.5f) + 1.0f) * 0.25f);
	}

    gl_Position = worldMatrix * vec4(vpos, 1.0);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	world_position =  (worldMatrix * vec4(vpos, 1.0)).xyz;
	world_normal = (worldMatrix * vec4(normalize(vertex_normal), 0.0f)).xyz;
	world_tangent = (worldMatrix * vec4(normalize(vertex_tangent), 0.0f)).xyz;
	material_info = vec4(vertex_texcoord, materialIndex, materialShininess);
}
