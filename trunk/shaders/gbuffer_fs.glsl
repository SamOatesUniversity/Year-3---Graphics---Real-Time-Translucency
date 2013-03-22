#version 330

in vec3 world_position;
in vec3 world_normal;
in vec3 world_tangent;
in vec2 world_texcoord;
in vec4 material_info;
in vec4 wvpVertexPosition;

out vec4 fragment_colour[4];

void main(void)
{
    fragment_colour[0] = vec4(world_position, wvpVertexPosition.z / wvpVertexPosition.w);
	fragment_colour[1] = vec4(world_normal, 1.0f);
	fragment_colour[2] = material_info;
	fragment_colour[3] = vec4(world_tangent, 1.0f);
}
