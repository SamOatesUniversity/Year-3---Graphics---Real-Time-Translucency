#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec4 vertexPos;

out vec4 fragment_colour;

void main(void)
{
	float depth = vertexPos.z / vertexPos.w;
	fragment_colour = vec4(depth, 0, 0, 1);
}
