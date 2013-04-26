#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 worldMatrix;  

in vec3 vertex_position;
in vec3 vertex_normal;

out vec4 lvpVertexPosition;
out vec4 lvpVertexNormal;

void main(void)
{
	lvpVertexNormal = vec4(normalize(vertex_normal), 0.0f);
	lvpVertexNormal = worldMatrix * lvpVertexNormal;

    lvpVertexPosition = worldMatrix * vec4(vertex_position, 1.0f);
	gl_Position = viewMatrix * lvpVertexPosition;
	gl_Position = projectionMatrix * gl_Position;
}
