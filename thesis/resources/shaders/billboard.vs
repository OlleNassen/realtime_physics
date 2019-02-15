#version 430

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texcoord;
layout (location = 2) in vec3 vertex_color;
layout (location = 3) in vec3 vertex_offset;

out vec3 vs_position;
out vec2 vs_texcoord;
out vec3 vs_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 model_view = view * model;

	vec4 new_offset = view * model * vec4(vertex_offset,1);

	model_view[0][0] = 1;
	model_view[0][1] = 0;
	model_view[0][2] = 0;

	model_view[1][0] = 0;
	model_view[1][1] = 1;
	model_view[1][2] = 0;

	model_view[2][0] = 0;
	model_view[2][1] = 0;
	model_view[2][2] = 1;
	
	vs_texcoord = vertex_texcoord;
    vs_color = vertex_color;
    gl_Position = projection * model_view * vec4(vertex_position + vec3(new_offset.xyz), 1.0);
}