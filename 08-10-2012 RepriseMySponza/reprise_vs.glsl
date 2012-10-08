#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 world_position;
out vec3 world_normal;

void main(void)
{
    gl_Position = worldMatrix * vec4(vertex_position, 1.0);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	world_position =  (worldMatrix * vec4(vertex_position, 1.0)).xyz;
	world_normal = (worldMatrix * vec4(normalize( vertex_normal ), 0.0f)).xyz;
}
