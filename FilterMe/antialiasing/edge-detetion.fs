vec3 edgeDetection(sampler2D sampler, ivec2 pixel_coord)
{
	mat3 G[2] = mat3[](
		mat3( 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 ),
		mat3( 1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0 )
	);

	mat3 I;
	float cnv[2];
	vec3 sample;
	
	/* fetch the 3x3 neighbourhood and use the RGB vector's length as intensity value */
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++) 
		{
			sample = texelFetch( sampler_pixel, pixel_coord + ivec2(i-1,j-1), 0 ).rgb;
			I[i][j] = length(sample); 
		}
	}
	
	/* calculate the convolution values for all the masks */
	for (int i=0; i<2; i++) 
	{
		float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
		cnv[i] = dp3 * dp3; 
	}

	vec4 color = vec4(0.5 * sqrt(cnv[0]*cnv[0]+cnv[1]*cnv[1]));
	return color.xyz;
}