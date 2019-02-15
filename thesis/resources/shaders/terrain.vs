#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture;

out vec3 normal;
out vec2 texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    normal = transpose(inverse(mat3(model))) * vertex_normal;
    texture = vertex_texture;
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
