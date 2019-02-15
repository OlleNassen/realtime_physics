#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <array>
#include <algorithm>
#include <functional>
#include "node.hpp"
#include "buffer.hpp"
#include "objects.hpp"

namespace scene
{
namespace particles
{

class particle
{
public:
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 color;
    float size;
    float angle;
    float weight;
    float life;

    bool is_alive() const {return life > 0;}
};

class emitter : public node
{
public:
	emitter();
	emitter(float x, float y, float z);

private:
	vertex_array v_array;

	buffer vertex_buffer;
	buffer position_buffer;
	buffer color_buffer;

    template<typename T>
    using p_array = std::array<T, 10000>;

    p_array<particle> particles;
    p_array<particle>::iterator last_used = particles.begin();
    p_array<glm::vec3> positions;
    p_array<glm::vec3> colors;

	void update_current(milliseconds delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;
};

}
}

#endif // PARTICLES_HPP
