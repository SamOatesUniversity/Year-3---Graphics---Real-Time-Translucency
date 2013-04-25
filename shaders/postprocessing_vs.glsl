#version 330

in vec3 vertex_position;
in vec2 vertex_texcoord;

out vec2 vertTexcoord;

void main(void)
{
    gl_Position = vec4(vertex_position, 1.0);
	vertTexcoord = (gl_Position.xy + 1.0f) * 0.5f;
}
