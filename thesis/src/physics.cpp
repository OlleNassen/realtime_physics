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

typedef glm::vec3 Point;
Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;

	float snom = glm::dot(p - a, ab);
	float sdenom = glm::dot(p - b, a - c);
	float tnom = glm::dot(p - a, c);
	float tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a;

	float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b;
	if (tdenom <= 0.0f && udenom <= 0.0f) return c;

	glm::vec3 n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));

	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	float va = glm::dot(n, glm::cross(b - p, c - p));

	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	float vb = glm::dot(n, glm::cross(c - p, a - p));

	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}

int TestSphereTriangle(sphere s, Point a, Point b, Point c, Point& p)
{
	p = ClosestPtPointTriangle(s.position, a, b, c);
	glm::vec3 v = p - s.position;
	return glm::dot(v, v) <= s.radius * s.radius;
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
		glm::vec3 closest;
		if (TestSphereTriangle(w->player_collider, triangle.x, triangle.y, triangle.z, closest))
		{
			w->player_position.position = triangle.x;
		}
	}
}