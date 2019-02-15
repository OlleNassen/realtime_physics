#version 430

in vec3 fragment_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec3 obj_color;
uniform vec3 light_color;

uniform vec3 light_pos;
uniform vec3 view_position; // World cordinates

void main()
{
	vec3 normalized_normal = normalize(normal);
	vec3 light_direction = normalize(light_pos - fragment_pos);
    vec3 view_direction = normalize(view_position - fragment_pos);
	vec3 reflection_direction = reflect(light_direction, normalized_normal);


    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normalized_normal, halfway_direction), 0.0), 256);
    vec3 specular = light_color * spec;


	float diff_value = max(dot(normalized_normal, light_direction), 0);
	vec3 diffuse = diff_value * light_color;


	vec3 ambient = light_color * 0.1; // 0.1 is ambient value

	vec3 result = (ambient + diffuse + specular) * obj_color;
	frag_color = vec4(result, 1);
}
