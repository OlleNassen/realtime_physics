#version 430

in vec2 texture_pos;
out vec4 fragment_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D roughness;
uniform sampler2D metallic;
uniform sampler2D ao;


void main()
{
    fragment_color = texture2D(albedo, texture_pos);
}

