#version 430

in vec3 position;
in vec3 normal;

out vec4 frag_color;

uniform vec3 view_position; // World cordinates
uniform samplerCube skybox;

void main()
{             
	//position is in world space
    vec3 view_dir = normalize(position - view_position);
    vec3 reflection = reflect(view_dir, normalize(normal));
    frag_color = vec4(texture(skybox, reflection).rgb, 1.0);
}