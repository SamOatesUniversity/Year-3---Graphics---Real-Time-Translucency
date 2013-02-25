#version 330

in vec2 varying_coord;

out vec4 fragment_colour;

void main(void)
{
    float angle = atan(varying_coord.x / varying_coord.y);
    float angular_block_size = 3.142 / 15.0;
    float angular_coord = mod(angle, angular_block_size) / angular_block_size;
    float angular_line_width = 0.2;
    if (angular_coord < angular_line_width) {
        discard;
    }

    vec3 colour = vec3(0.5 + 0.5 * cos(varying_coord.x),
                       0.5 + 0.5 * sin(varying_coord.y),
                       0);
    
    float block_size = 0.1;
    vec2 coord = mod(varying_coord, block_size) / block_size;
    float line_width = 0.15;
    if (coord.x < line_width || coord.y < line_width) {
        colour = vec3(1, 0, 0);
    }

    fragment_colour = vec4(colour, 1.0);
}
