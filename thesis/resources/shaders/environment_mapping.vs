#version 430
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

out vec3 position;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    position = vec3(model * vec4(vertex_position, 1.0));
    normal = mat3(transpose(inverse(model))) * vertex_normal;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}  