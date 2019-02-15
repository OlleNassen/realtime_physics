#ifndef ICOSAHEDRON_HPP
#define ICOSAHEDRON_HPP

#include "node.hpp"
#include "buffer.hpp"

#include "texture.hpp"

namespace scene
{

class icosahedron : public node
{
public:
	icosahedron(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	void set_color(const glm::vec3& color);
	void set_tessellation(float distance);

private:
	vertex_array v_array;
	buffer vbo;
	buffer ebo{GL_ELEMENT_ARRAY_BUFFER};

	float tessellation_level{2};

	glm::vec3 temp_color;

	unsigned int index_count;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
