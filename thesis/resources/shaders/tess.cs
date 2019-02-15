#version 430

layout(vertices = 3) out;
in vec3 position[];
out vec3 cs_position[];
uniform float inner_level;
uniform float outer_level;

#define ID gl_InvocationID

void main()
{
    cs_position[ID] = position[ID];
    if (ID == 0)
    {
        gl_TessLevelInner[0] = inner_level;

        gl_TessLevelOuter[0] = outer_level;
        gl_TessLevelOuter[1] = outer_level;
        gl_TessLevelOuter[2] = outer_level;
    }
}