#include "quad_tess.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{

quad_tess::quad_tess(float x, float y, float z)
	: node(x, y, z)
	, tess_texture("resources/images/brick_texture.jpg",
        wrap::REPEAT, filter::LINEAR, format::RGB)
	, displacement_map("resources/images/diff_disp.png",
        wrap::REPEAT, filter::LINEAR, format::RGB)
{
	constexpr float vertices[] =
	{
		//pos, uv
		-0.5f, -0.5f, 0.f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.f, 1.0f, 1.0f,
	};

	constexpr int indices[] =
	{
		0, 1, 2, 1, 3, 2
	};

	quad_array.bind();
	quad_vbo.data(sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	quad_ebo.data(sizeof(indices), &indices[0], GL_STATIC_DRAW);

	quad_array.attribute_pointer(0, 3, GL_FLOAT,
        GL_FALSE, 5 * sizeof(float), nullptr);
	quad_array.attribute_pointer(1, 2, GL_FLOAT,
        GL_FALSE, 5 * sizeof(float), buffer_offset<float>(3u));
}

void quad_tess::update_current(float delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
}

void quad_tess::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	shader.uniform("TessLevelInner", 2.f);
	shader.uniform("TessLevelOuter", 1.f);

	tess_texture.uniform(shader, "diffuse", 0);

	quad_array.bind();
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
