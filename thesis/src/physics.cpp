#include "physics.hpp"
#include "GL/glew.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float magnitude_squared(glm::vec3* v)
{
	return glm::dot(*v, *v);
}

bool sphere_triangle(sphere* sphere, triangle* triangle)
{
	glm::vec3 A = triangle->x - sphere->position;
	glm::vec3 B = triangle->y - sphere->position;
	glm::vec3 C = triangle->z - sphere->position;
	float rr = sphere->radius * sphere->radius;
	glm::vec3 V = cross(B - A, C - A);
	float d = dot(A, V);
	float e = dot(V, V);
	bool sep1 = d * d > rr * e;
	float aa = dot(A, A);
	float ab = dot(A, B);
	float ac = dot(A, C);
	float bb = dot(B, B);
	float bc = dot(B, C);
	float cc = dot(C, C);
	bool sep2 = (aa > rr) & (ab > aa) & (ac > aa);
	bool sep3 = (bb > rr) & (ab > bb) & (bc > bb);
	bool sep4 = (cc > rr) & (ac > cc) & (bc > cc);
	glm::vec3 AB = B - A;
	glm::vec3 BC = C - B;
	glm::vec3 CA = A - C;
	float d1 = ab - aa;
	float d2 = bc - bb;
	float d3 = ac - cc;
	float e1 = dot(AB, AB);
	float e2 = dot(BC, BC);
	float e3 = dot(CA, CA);
	glm::vec3 Q1 = A * e1 - d1 * AB;
	glm::vec3 Q2 = B * e2 - d2 * BC;
	glm::vec3 Q3 = C * e3 - d3 * CA;
	glm::vec3 QC = C * e1 - Q1;
	glm::vec3 QA = A * e2 - Q2;
	glm::vec3 QB = B * e3 - Q3;
	bool sep5 = (dot(Q1, Q1) > rr * e1 * e1) & (dot(Q1, QC) > 0);
	bool sep6 = (dot(Q2, Q2) > rr * e2 * e2) & (dot(Q2, QA) > 0);
	bool sep7 = (dot(Q3, Q3) > rr * e3 * e3) & (dot(Q3, QB) > 0);
	bool separated = sep1 | sep2 | sep3 | sep4 | sep5 | sep6 | sep7;

	return separated;
}

void draw(world* w, const shader& shader)
{
	static bool run_once = false;
	static unsigned int m_vao;
	static unsigned int m_vbo;
	static glm::mat4 model(1.f);
	if (!run_once)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle) * w->triangles.size(), &w->triangles[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		run_once = true;
	}
	shader.uniform("model", model);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, w->triangles.size() * 3);
	glBindVertexArray(0);
}

void collision(world* w)
{
	w->player_collider.position = w->player_position.position;
	
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
	bool collision = false;

	for (auto& triangle : w->triangles)
	{
		if (!sphere_triangle(&w->player_collider, &triangle))
		{
			collision = true;
			glm::vec3 a = triangle.y - triangle.x;
			glm::vec3 b = triangle.z - triangle.x;
			normal += glm::normalize(glm::cross(a, b));
		}
	}

	if (collision)
	{
		normal = glm::normalize(normal);
		float elasticity = 0.7f;
		float sphere_weight = 10.0f;
		float friction_val = 0.09f;

		glm::vec3 gravity_direction = glm::normalize(w->gravity);
		glm::vec3 force = sphere_weight * w->gravity;
		float angle = glm::dot(w->gravity, normal);

		glm::vec3 normal_force = force * normal * angle;
		glm::vec3 friction = normal_force * -friction_val;

		glm::vec3 total_forces(0.f);
		total_forces += normal_force;
		total_forces += friction;

		glm::vec3 total_displacement = (total_forces / sphere_weight) * w->dt * w->dt;

		glm::vec3 new_pos = w->player_position.position + total_displacement + elasticity * glm::reflect(
			w->player_position.velocity * w->dt,
			normal);

		w->player_position.old_position = w->player_position.position;
		w->player_position.position = new_pos;

		w->player_position.velocity = (w->player_position.position - w->player_position.old_position) / w->dt;
	}
}

void update_verlet(world* w)
{
	glm::vec3 acceleration = w->player_position.acceleration + w->gravity;
	glm::vec3 temp_position = w->player_position.position;
	std::cout << w->player_position.velocity.y << '\n';
	
	w->player_position.position =
		2.0f * w->player_position.position - w->player_position.old_position 
		+ acceleration * w->dt * w->dt;

	w->player_position.velocity = 
		(w->player_position.position - w->player_position.old_position) / w->dt;
	
	
	w->player_position.old_position = temp_position;
	collision(w);
}

void update_euler(world* w)
{
	glm::vec3 acceleration = w->player_position.acceleration + w->gravity;
	glm::vec3 temp_position = w->player_position.position;
	std::cout << w->player_position.velocity.y << '\n';
	
	w->player_position.velocity += acceleration * w->dt;
	w->player_position.position += w->player_position.velocity * w->dt;

	
	w->player_position.old_position = temp_position;
	collision(w);
}