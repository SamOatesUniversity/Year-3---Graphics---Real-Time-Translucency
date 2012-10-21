#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec3 vertex_position;

out vec4 vertexPos;

void main(void)
{
    vertexPos = gl_Position = projectionMatrix * viewMatrix * (worldMatrix * vec4(vertex_position, 1.0));
}
