#include "mesh.hpp"

mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<texture>& textures)
:vbo(GL_ARRAY_BUFFER),
ebo(GL_ELEMENT_ARRAY_BUFFER)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setup_mesh();
}

mesh::~mesh()
{
}

void mesh::draw(const shader & shader)
{
	if (indices.size() > 0)
	{
		vao.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}

void mesh::setup_mesh()
{
	vao.bind();
	vbo.data(vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
	//ebo.data(indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	vao.attribute_pointer(0, 3, GL_FLOAT,
		GL_FALSE, sizeof(vertex), nullptr);
	vao.attribute_pointer(1, 2, GL_FLOAT,
		GL_FALSE, sizeof(vertex), scene::buffer_offset<float>(3u));
	vao.attribute_pointer(2, 3, GL_FLOAT,
		GL_FALSE, sizeof(vertex), scene::buffer_offset<float>(5u));
}
