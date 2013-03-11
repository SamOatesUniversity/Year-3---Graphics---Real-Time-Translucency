#include "antialiasing/edge-detetion.fs"

vec3 simpleBlur(sampler2D sampler_pixel, ivec2 pixel_coord, int kernal)
{
	vec3 pixel_colour;
	int noofSamples = 0;
	
	for (int x = -kernal; x <= kernal; ++x)
	{
		for (int y = -kernal; y <= kernal; ++y)
		{
			ivec2 offset = ivec2(x, y);
			pixel_colour += texelFetch(sampler_pixel, pixel_coord + offset, 0).rgb;
			noofSamples++;
		}
	}
	
	return pixel_colour / noofSamples;
}

vec3 edgeBlur( sampler2D sampler_pixel, ivec2 pixel_coord, int kernal)
{
	vec3 edge_color = edgeDetection(sampler_pixel, pixel_coord);

	if (edge_color.x == 0)
	{
		return texelFetch(sampler_pixel, pixel_coord, 0).rgb;
	}

	vec3 pixel_colour = vec3(0, 0, 0);
	int noofSamples = 0;
	
	for (int x = -kernal; x <= kernal; ++x)
	{
		for (int y = -kernal; y <= kernal; ++y)
		{
			ivec2 offset = ivec2(x, y);
			pixel_colour += texelFetch(sampler_pixel, pixel_coord + offset, 0).rgb;
			noofSamples++;
		}
	}
	
	return pixel_colour / noofSamples;
}