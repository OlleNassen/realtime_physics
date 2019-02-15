#ifndef NORMAL_QUAD_HPP
#define NORMAL_QUAD_HPP

#include "node.hpp"
#include "buffer.hpp"

#include "texture.hpp"

namespace scene
{

class normal_quad : public node
{
public:
	normal_quad(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	~normal_quad();

	void set_texture(const std::string& path);
private:
	vertex_array quad_array;
	buffer quad_vbo;
	texture* quad_texture;
	texture* quad_normal;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

}

#endif
