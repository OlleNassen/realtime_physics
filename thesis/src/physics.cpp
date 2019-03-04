#include "physics.hpp"
#include "GL/glew.h"
#include <iostream>

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

	if (!run_once)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(w->triangles), &w->triangles[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		run_once = true;
	}

	shader.use();
	shader.uniform("model", glm::mat4(1.f));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, w->triangles.size());
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
		if (!sphere_triangle(&w->player_collider, &triangle))
		{
			glm::vec3 a = triangle.y - triangle.x;
			glm::vec3 b = triangle.z - triangle.x;

			glm::vec3 new_pos = w->player_position.old_position +
				glm::reflect(
					w->player_position.position - w->player_position.old_position,
					glm::normalize(glm::cross(a, b)));

			w->player_position.position = new_pos;
		}
	}
}