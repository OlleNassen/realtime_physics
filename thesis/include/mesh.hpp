#ifndef MESH_HPP
#define MESH_HPP
#include <vector>
#include "texture.hpp"
#include "shader.hpp"

#include "buffer.hpp"
#include "node.hpp"

struct vertex 
{
	glm::vec3 position;
	glm::vec2 texcoords;
	glm::vec3 normal;
	vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3 nor)
	{
		position = pos;
		texcoords = tex;
		normal = nor;
	}
};

class mesh
{
public:
	mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<texture>& textures);
	virtual ~mesh();
	virtual void draw(const shader& shader);

	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<texture> textures;
protected:
	vertex_array vao;
	buffer vbo;
	buffer ebo;
	virtual void setup_mesh();
};

class md5_mesh: public mesh
{

};

#endif