#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "texture.hpp"

namespace scene
{

class terrain : public node
{
public:
	terrain(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	glm::vec3 calculate_camera_position(
        const glm::vec3& world_position, float position_offset) const;


private:
	std::vector<int> heights;
	int draw_count{0};

	glm::mat4 inverse_transform{1.0f};

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo{GL_ELEMENT_ARRAY_BUFFER};
	texture grass{"resources/images/grass.png",
        wrap::REPEAT, filter::LINEAR, format::RGBA};
	texture slope{"resources/images/slope.png",
        wrap::REPEAT, filter::LINEAR, format::RGBA};
	texture rock{"resources/images/rock.png",
        wrap::REPEAT, filter::LINEAR, format::RGBA};

	int width = 0;
	int height = 0;
	int channels = 0;
	float height_offset;

	void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

}

#endif
