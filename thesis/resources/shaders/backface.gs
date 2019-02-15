#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position[];

void main()
{
	vec3 uno = normalize(position[1].xyz - position[0].xyz);
	vec3 dos = normalize(position[2].xyz - position[0].xyz);

	vec3 cross_product = cross(uno, dos);

	if(dot(cross_product, -position[0].xyz) > 0.0)
	{
		for(int i = 0;i < 3;i++)
	    {
			gl_Position = projection * position[i];
			EmitVertex();
	    }
	    EndPrimitive();
	}

}
