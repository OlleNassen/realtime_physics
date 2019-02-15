#ifndef QUAD_TESS_HPP
#define QUAD_TESS_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "texture.hpp"

namespace scene
{

class quad_tess : public node
{
public:
	quad_tess(float x = 0.0f, float y = 0.0f, float z = 0.0f);

private:
	vertex_array quad_array;
	buffer quad_vbo;
	buffer quad_ebo{GL_ELEMENT_ARRAY_BUFFER};
	texture tess_texture;
	texture displacement_map;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
