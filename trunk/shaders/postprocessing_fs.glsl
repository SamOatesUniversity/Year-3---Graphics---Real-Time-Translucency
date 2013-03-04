#version 330

#include "antialiasing/blur.fs"

uniform sampler2D sampler_pixel;
out vec4 fragment_colour;

void main(void)
{
    ivec2 pixel_coord = ivec2(gl_FragCoord.xy);

    //vec3 pixel_colour = simpleBlur(sampler_pixel, pixel_coord, 6);
	//vec3 pixel_colour = edgeBlur(sampler_pixel, pixel_coord, 1);
	vec3 pixel_colour = edgeDetection(sampler_pixel, pixel_coord);
	
    fragment_colour = vec4(pixel_colour, 0.0f);
}
