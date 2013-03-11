#version 330

#include "antialiasing/blur.fs"

#define SMAA_PIXEL_SIZE float2(1.0 / 1280.0, 1.0 / 720.0)
#define SMAA_GLSL_3 1
#define SMAA_PRESET_LOW 1

#include "antialiasing/SMAA.fs"

uniform sampler2D sampler_pixel;
out vec4 fragment_colour;

void main(void)
{
    ivec2 pixel_coord = ivec2(gl_FragCoord.xy);
    vec3 pixel_colour = edgeBlur(sampler_pixel, pixel_coord, 1);
    fragment_colour = vec4(pixel_colour, 0.0f);
}
