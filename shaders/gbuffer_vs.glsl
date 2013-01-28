#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

uniform vec3 materialColor;
uniform float materialShininess; 

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;

out vec3 world_position;
out vec3 world_normal;
out vec3 world_tangent;
out vec2 world_texcoord;
out vec4 material_info;

void main(void)
{
    gl_Position = worldMatrix * vec4(vertex_position, 1.0);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	world_position =  (worldMatrix * vec4(vertex_position, 1.0)).xyz;
	world_normal = (worldMatrix * vec4(normalize(vertex_normal), 0.0f)).xyz;
	world_tangent = (worldMatrix * vec4(normalize(vertex_tangent), 0.0f)).xyz;
	world_texcoord = vertex_texcoord;
	material_info = vec4(materialColor, materialShininess);
}
