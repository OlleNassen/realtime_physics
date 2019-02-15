#version 430
layout (location = 0) in vec4 vertex; // <vec2 pos and vec2 tex>

out vec2 tex_coord;

uniform mat4 projection;

void main()
{
    tex_coord = vertex.zw;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}  