#version 430

layout(triangles, equal_spacing, cw) in;

in vec3 cs_position[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 p0 = gl_TessCoord.x * cs_position[0];
    vec3 p1 = gl_TessCoord.y * cs_position[1];
    vec3 p2 = gl_TessCoord.z * cs_position[2];

    vec3 position = normalize(p0 + p1 + p2);
    gl_Position = projection * view * model * vec4(position, 1);
}