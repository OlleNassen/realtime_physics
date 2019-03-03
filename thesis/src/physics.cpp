#include "physics.hpp"

float magnitude_squared(glm::vec3* v)
{
	return glm::dot(*v, *v);
}

glm::vec3 closest_point(plane* plane, glm::vec3* point)
{
	float dot = glm::dot(plane->normal, *point);
	float distance = dot - glm::dot(plane->position, plane->normal);
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

bool sphere_triangle(sphere* sphere, triangle* triangle)
{
	glm::vec3 A = triangle->x - sphere->position;
	glm::vec3 B = triangle->y - sphere->position;
	glm::vec3 C = triangle->z - sphere->position;

	float aa = glm::dot(A, A);
	float ab = glm::dot(A, B);
	float ac = glm::dot(A, C);
	float bb = glm::dot(B, B);
	float bc = glm::dot(B, C);
	float cc = glm::dot(C, C);
	float rr = sphere->radius * sphere->radius;

	bool separateda = (aa > rr) & (ab > aa) & (ac > aa);
	bool separatedb = (bb > rr) & (ab > bb) & (bc > bb);
	bool separatedc = (cc > rr) & (ac > cc) & (bc > cc);
	bool separated = separateda | separatedb | separatedc;

	return separated;
}

void update_verlet(world* w)
{
	float dt_squared = w->dt * w->dt;

	glm::vec3 acceleration;
	acceleration.x = 0.0f;
	acceleration.y = -9.82f;
	acceleration.z = 0.0f;

	//Position
	glm::vec3 temp_position = w->player_position.position;

	w->player_position.position =
		2.0f * w->player_position.position
		- w->player_position.old_position +
		acceleration * w->dt * w->dt;

	acceleration.y = 0.0f;
	
	//Rotation	
	glm::vec3 temp_rotation = w->player_position.rotation;

	w->player_position.rotation =
		2.0f * w->player_position.rotation
		- w->player_position.old_rotation +
		acceleration * w->dt * w->dt;

	//resolve collisions
	w->player_collider.position = w->player_position.position;

	w->player_position.old_position = temp_position;
	w->player_position.old_rotation = temp_rotation;

	for (auto& triangle : w->triangles)
	{
		if (sphere_triangle(&w->player_collider, &triangle))
		{
			w->player_collider.position = triangle.x;
		}
	}
}