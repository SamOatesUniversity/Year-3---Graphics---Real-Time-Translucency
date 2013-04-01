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
	lvpVertexNormal = worldMatrix * vec4(normalize(vertex_normal), 0.0f);
	lvpVertexNormal = viewMatrix * lvpVertexNormal;

    lvpVertexPosition = worldMatrix * vec4(vertex_position, 1.0f);
	lvpVertexPosition = viewMatrix * lvpVertexPosition;
	lvpVertexPosition = projectionMatrix * lvpVertexPosition;
	gl_Position = lvpVertexPosition;
}
