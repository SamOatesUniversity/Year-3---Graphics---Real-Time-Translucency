#version 330

uniform mat4 projection_view_model_xform;

in vec3 vertex_position;

out vec2 varying_coord;

void main(void)
{
    varying_coord = vertex_position.xy;
    gl_Position = projection_view_model_xform * vec4(vertex_position, 1.0);
}
