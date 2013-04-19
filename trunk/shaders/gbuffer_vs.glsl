#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

uniform int materialIndex;
uniform float materialShininess; 

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texcoord;

out vec3 world_position;
out vec3 world_normal;
out vec4 material_info;

void main(void)
{
    gl_Position = worldMatrix * vec4(vertex_position, 1.0);
	world_position =  (gl_Position).xyz;

	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	world_normal = (worldMatrix * vec4(normalize(vertex_normal), 0.0f)).xyz;
	material_info = vec4(vertex_texcoord, materialIndex, materialShininess);
}
