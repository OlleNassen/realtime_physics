#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 patch_distance[3];

out vec3 geo_patch_distance;
out vec3 geo_tri_distance;

uniform mat4 model;
uniform mat4 view;

void main()
{    
    geo_patch_distance = patch_distance[0];
    geo_tri_distance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position;
	EmitVertex();

    geo_patch_distance = patch_distance[1];
    geo_tri_distance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position;
	EmitVertex();

    geo_patch_distance = patch_distance[2];
    geo_tri_distance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position;
	EmitVertex();

    EndPrimitive();
}
