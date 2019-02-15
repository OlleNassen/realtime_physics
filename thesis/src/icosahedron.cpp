#include "icosahedron.hpp"
#include <iostream>

namespace scene
{

icosahedron::icosahedron(float x, float y, float z)
    : node(x, y, z)
{
	temp_color = glm::vec3(1.0f, 0.1f, 0.1f);
	constexpr int faces[] = {
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,
		11, 6,  7,
		11, 7,  8,
		11, 8,  9,
		11, 9,  10,
		11, 10, 6,
		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,
		2,  7, 6,
		3,  8, 7,
		4,  9, 8,
		5, 10, 9,
		1, 6, 10 };

	constexpr  float verts[] = {
		0.000f,  0.000f,  1.000f,
		0.894f,  0.000f,  0.447f,
		0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		0.276f, -0.851f,  0.447f,
		0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		0.724f, -0.526f, -0.447f,
		0.000f,  0.000f, -1.000f };

	index_count = sizeof(faces) / sizeof(faces[0]);

	// Create the VAO:
	v_array.bind();

	vbo.data(sizeof(verts), &verts[0], GL_STATIC_DRAW);
	ebo.data(sizeof(faces), &faces[0], GL_STATIC_DRAW);

	v_array.attribute_pointer(0, 3, GL_FLOAT,
        GL_FALSE, 3 * sizeof(float), nullptr);
}

void icosahedron::set_color(const glm::vec3& color)
{
	temp_color = color;
}

void icosahedron::set_tessellation(float distance)
{
	if (distance < 5)
	{
		tessellation_level = 4;
	}

	else if (distance < 10)
	{
		tessellation_level = 2;
	}
}

void icosahedron::update_current(milliseconds delta_time,
    const glm::mat4 & world_transform, glm::mat4 & transform)
{
}

void icosahedron::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	shader.uniform("color", temp_color);
	shader.uniform("inner_level", tessellation_level);
	shader.uniform("outer_level", tessellation_level * 2);
	v_array.bind();
	glDrawElements(GL_PATCHES, index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
