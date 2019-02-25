#include "physics.hpp"

float magnitude_squared(glm::vec3* v)
{
	return glm::dot(*v, *v);
}

glm::vec3 closest_point(plane* plane, glm::vec3* point)
{
	float dot = glm::dot(plane->normal, *point);
	float distance = dot - plane->distance;
	return *point - plane->normal * distance;
}

bool sphere_plane(sphere* sphere, plane* plane)
{
	glm::vec3 closest = closest_point(plane, &sphere->position);
	float distance_squared =
		magnitude_squared(&(sphere->position - closest));

	float radius_squared = sphere->radius * sphere->radius;
	return distance_squared < radius_squared;

}

void update(world* w)
{
	float dt_squared = w->dt * w->dt;

	glm::vec3 temp = w->player_position.position;

	w->player_position.position =
		2.0f * w->player_position.position
		- w->player_position.old_position +
		w->player_position.acceleration * w->dt * w->dt;

	w->player_position.old_position = temp;

	//resolve collisions
}