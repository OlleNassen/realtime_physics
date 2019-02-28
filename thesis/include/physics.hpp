#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <glm/glm.hpp>

struct point
{
	glm::vec3 position;
	glm::vec3 old_position;
	glm::vec3 acceleration;
};

struct sphere
{
	glm::vec3 position;
	float radius;
};

struct plane
{
	glm::vec3 normal;
	float distance;
};

struct world
{
	float dt;
	point player_position;
	sphere player_collider;
	plane planes[100];
};

float magnitude_squared(glm::vec3* v);
glm::vec3 closest_point(plane* plane, glm::vec3* point);
bool sphere_plane(sphere* sphere, plane* plane);

void update_verlet(world* w);

#endif
