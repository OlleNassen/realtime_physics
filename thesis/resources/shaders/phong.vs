#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 fragment_pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertex_position, 1);

	normal = transpose(inverse(mat3(model)))
        * vertex_normal; // Normal matrix * normal (worldspace fix)
	fragment_pos = vec3(model * vec4(vertex_position, 1));
}
