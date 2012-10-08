/*
 * Tutorial: This is the fragment shader.  The value written to the colour
 *           variable will be written to the pixel in the framebuffer. The
 *           colour value must be RGBA.
 */

#version 330

uniform float time_seconds;

in vec3 something;

out vec4 fragment_colour;

void main(void)
{
    fragment_colour = vec4(1.0, 0.0, 0.0, 1.0);
}
