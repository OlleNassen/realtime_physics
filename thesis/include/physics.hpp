#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"
#include "cam3p.hpp"

struct point
{
	glm::vec3 position;
	glm::vec3 old_position;

	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct sphere
{
	glm::vec3 position;
	float radius;
	float weight;
	float elasticity;
};

struct triangle
{
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
};

struct world
{
	float friction_val = 0.09f;
	float dt;
	point player_position;
	sphere player_collider;
	point enemy_position;
	sphere enemy_collider;
	glm::vec3 gravity;
	glm::mat4 model_mat = glm::mat4(1.f);
	std::vector<triangle> triangles;	
};

void draw(world* w, const shader& shader);

float magnitude_squared(glm::vec3* v);

void collision(world* w);
void update_verlet(world* w);
void update_euler(world* w);

void update_verlet(point* p, glm::vec3 gravity, float dt);
void update_euler(point* p, glm::vec3 gravity, float dt);

#endif
