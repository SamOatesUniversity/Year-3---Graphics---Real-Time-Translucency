/*
 * Tutorial: This is the vertex shader.  The value written to the gl_Position
 *           variable will be used as the vertex position by the rasterizer.
 *           The gl_Position value must be XYZW.
 */

#version 330

uniform float time_seconds;
uniform mat4 mystery_xform;

in vec3 vertex_position;

out vec3 something;

void main(void)
{
	something = vec3(0.0);
    gl_Position = vec4(vertex_position, 1.0) * mystery_xform;
}
