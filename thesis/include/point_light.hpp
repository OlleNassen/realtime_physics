#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "node.hpp"
#include "buffer.hpp"

#include "texture.hpp"
#include "box.hpp"

namespace scene
{

class point_light: public node
{
public:
	point_light();

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
	box* da_box; // lol

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}

#endif
