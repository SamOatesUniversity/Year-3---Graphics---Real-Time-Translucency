#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec4 vertexPos;

out vec4 fragment_colour;

void main(void)
{
	float depth = vertexPos.z / vertexPos.w;
	
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	
	fragment_colour = vec4( depth, depth*depth + 0.25*(dx*dx + dy*dy), 0, 1 );
}
