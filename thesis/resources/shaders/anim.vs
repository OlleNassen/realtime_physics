#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 joint;
layout (location = 4) in vec4 weight;

out vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.world_pos = (model * vec4(position, 1)).xyz;
    vs_out.tex_coord = texture_coordinate;
	vs_out.world_normal = (model * vec4(normal, 0)).xyz;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
