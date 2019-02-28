#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 joint;
layout (location = 4) in vec4 weight;

out vec2 texture_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texture_pos = texture_coordinate;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
