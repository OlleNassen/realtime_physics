#version 430

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texture_coordinate_in;
layout (location = 2) in vec3 normal_in;

out vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    position = view * model * vec4(position_in, 1.0);
}
