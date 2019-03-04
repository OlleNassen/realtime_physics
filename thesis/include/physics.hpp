#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <glm/glm.hpp>
#include <vector>

struct point
{
	glm::vec3 position;
	glm::vec3 old_position;
	
	glm::vec3 rotation;
	glm::vec3 old_rotation;
};

struct sphere
{
	glm::vec3 position;
	float radius;
};

struct triangle
{
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
};

struct world
{
	float dt;
	point player_position;
	sphere player_collider;
	std::vector<triangle> triangles;
};

float magnitude_squared(glm::vec3* v);

void update_verlet(world* w);

#endif
