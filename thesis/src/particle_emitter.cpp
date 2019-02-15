#include "particle_emitter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <iostream>

namespace scene
{

particle_emitter::particle_emitter(float x, float y, float z)
	: node(x, y, z)
	, quad_texture(new texture("resources/images/edvard.png"))
{
	srand(time(NULL));

	for(auto& offset : offsets)
    {
        offset = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    for(auto& up : going_up)
    {
        up = true;
    }

	instance_vbo.data(sizeof(glm::vec3) * offsets.size(), offsets.data(),
        GL_STATIC_DRAW);
	constexpr auto size = 0.3f;
	constexpr float vertices[] =
	{
		// positions //Texcoords     // colors
		0.f,  size, 0.f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, 0.f, 0.f, 1.0f, 0.0,  0.0f, 1.0f, 0.0f,
		0.f, 0.f, 0.f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		0.f,  size, 0.f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size, size, 0.f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		size,  0.f, 0.f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	quad_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), nullptr);
	auto offset = 3u;
	quad_array.attribute_pointer(1, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), buffer_offset<float>(offset));
	offset += 2u;
	quad_array.attribute_pointer(2, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), buffer_offset<float>(offset));

	instance_vbo.bind();
	quad_array.attribute_pointer(3, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), nullptr);
	quad_array.attribute_divisor(3, 1);
}

void particle_emitter::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
	instance_vbo.data(sizeof(glm::vec3) * offsets.size(),
        offsets.data(), GL_STATIC_DRAW);

	for (auto i = 0u; i < offsets.size(); ++i)
	{
		glm::vec3 random(
            rand() % 100 - 50,
            rand() % 100,
            rand() % 100 - 50);

		if (offsets[i].y < 100 && going_up[i])
		{
			random /= glm::vec3(1000, 100, 1000);
			offsets[i] += random;
		}
		else if (offsets[i].y < 200 && going_up[i])
		{
			random /= glm::vec3(100, 100, 100);
			offsets[i] += random;
		}
		else if (going_up[i])
		{
			going_up[i] = offsets[i].y <= 400;
            random /= glm::vec3(50, 100, 50);
			offsets[i] += random;
		}
		else if (!going_up[i])
		{
			going_up[i] = offsets[i].y < 0;

			if (going_up[i])
			{
				offsets[i] = glm::vec3(0, 0, 0);
			}

			random /= glm::vec3(100, 50, 100);
			offsets[i] += glm::vec3(random.x, -random.y, random.z);
		}
	}
}

void particle_emitter::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "diffuseMap", 0);
	quad_array.bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offsets.size());
	glBindVertexArray(0);
}

void particle_emitter::set_texture(const std::string & path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}

}
