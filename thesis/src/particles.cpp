#include "particles.hpp"

namespace scene
{
namespace particles
{

emitter::emitter()
{
    vertex_buffer.data(object::quad, GL_STATIC_DRAW);
    v_array.attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    position_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    v_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    color_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    v_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void emitter::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
    using namespace std::chrono;
	using float_seconds = duration<float>;
	float delta = duration_cast<float_seconds>(delta_time).count();

    int new_particles = delta_time.count();
    for (int i = 0; i < new_particles; ++i)
    {
        if(++last_used == particles.end()) last_used = particles.begin();
        auto& p = *std::find_if_not(last_used, particles.end(),
            std::mem_fn(&particle::is_alive));

        p.life = 9.8f;
        p.position = glm::vec3(0.0f, 0.0f, 0.0f);

        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

        glm::vec3 randomdir
        {
            (rand()%2000 - 1000)/1000.0f,
            (rand()%2000 - 1000)/1000.0f,
            (rand()%2000 - 1000)/1000.0f
        };

        p.speed = maindir + randomdir * spread;

        p.color = {
            (rand() % 100)/100.0f,
            (rand() % 100)/100.0f,
            (rand() % 100)/100.0f};
        p.size = (rand()%1000)/2000.0f + 0.1f;
    }

    int p_count = 0;
    for (auto& p : particles)
    {
        p.life -= delta;
        if(p.is_alive())
        {
            p.speed += glm::vec3{0.0f, 9.8f, 0.0f} * delta;
            p.position += p.speed * delta;
            positions[p_count] = p.position;
            colors[p_count] = p.color;
        }

        ++p_count;
    }

    position_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    position_buffer.sub_data(0, sizeof(glm::vec3) * positions.size(),
        positions.data());

    color_buffer.data(sizeof(glm::vec3) * particles.size(),
        nullptr, GL_STREAM_DRAW);
    color_buffer.sub_data(0, sizeof(glm::vec3) * colors.size(),
        colors.data());
}

void emitter::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	vertex_buffer.bind();
    v_array.attribute_pointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    position_buffer.bind();
    v_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    color_buffer.bind();
    v_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles.size());

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

}
}
