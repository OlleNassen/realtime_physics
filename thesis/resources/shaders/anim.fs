#version 430

in vec2 texture_pos;
out vec4 fragment_color;

uniform sampler2D diffuse;

void main()
{
    fragment_color = texture2D(diffuse, texture_pos);
}

