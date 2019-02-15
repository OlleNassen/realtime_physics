#version 430

in vec2 vs_position;
in vec2 vs_texcoord;
in vec3 vs_color;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform vec4 color;

void main()
{
   	fragColor = texture2D(diffuseMap, vs_texcoord) * vec4(1,0.1,0.1, 1);
}
