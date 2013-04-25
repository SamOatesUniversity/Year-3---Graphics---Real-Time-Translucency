#version 330

#include "antialiasing/fxaa.fs"

uniform sampler2D sampler_pixel;
uniform vec2 texcoordOffset;
uniform bool enabled = true;

in vec2 vertTexcoord;
out vec4 fragment_colour;

void main(void)
{
	vec4 pixel_colour;
	if (enabled == true) {
		pixel_colour = FXAAProcess(sampler_pixel, vertTexcoord, texcoordOffset);
	} else {
		pixel_colour = texture2D(sampler_pixel, vertTexcoord);
	}
    fragment_colour = pixel_colour;
}
