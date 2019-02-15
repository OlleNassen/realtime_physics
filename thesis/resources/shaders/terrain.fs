#version 430

in vec3 normal;
in vec2 texture;

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;

out vec4 fragment_color;

void main()
{
	vec4 color1 = texture2D(diffuse, texture);
	vec4 color2 = texture2D(diffuse2, texture);
	vec4 color3 = texture2D(diffuse3, texture);
	float blend = 0.0;

	vec3 normalized_normal = normalize(normal);
	float slope = 1.0 - normalized_normal.y;

    if (slope < 0.2)
    {
        blend = slope / 0.2;
        fragment_color = mix(color1, color2, blend);
    }

    if (slope < 0.7 && slope >= 0.2)
    {
        blend = (slope - 0.2f) * (1.0 / (0.7 - 0.2));
        fragment_color = mix(color2, color3, blend);
    }

    if (slope >= 0.7)
    {
        fragment_color = color3;
    }
}
