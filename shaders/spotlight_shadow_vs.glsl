#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec3 vertex_position;
out vec4 world_position;

void main(void)
{
    gl_Position = worldMatrix * vec4(vertex_position, 1.0);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;
	
	world_position = gl_Position;
}
