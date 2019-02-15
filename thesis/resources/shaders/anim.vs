#version 430

#define MAX_JOINTS 50
#define MAX_WEIGHTS 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 joint;
layout (location = 4) in vec4 weight;

out vec2 texture_pos;

uniform mat4 joint_transforms[MAX_JOINTS];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 matrix = joint_transforms[joint[0]] * weight[0];
    matrix += joint_transforms[joint[1]] * weight[1];
    matrix += joint_transforms[joint[2]] * weight[2];
    matrix += joint_transforms[joint[3]] * weight[3];

    texture_pos = texture_coordinate;

    gl_Position = projection * view * model * matrix * vec4(position, 1.0);
}
