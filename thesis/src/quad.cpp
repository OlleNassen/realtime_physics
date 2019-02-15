#include "quad.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{

quad::quad(float x, float y, float z)
	: node(x, y, z)
    , quad_texture(new texture("resources/images/edvard.png"))
{
	quad_array.bind();
	quad_vbo.data(object::quad_uv, GL_STATIC_DRAW);
	quad_array.attribute_pointer(0, 2, GL_FLOAT,
        GL_FALSE, 5 * sizeof(float), nullptr);
	quad_array.attribute_pointer(1, 2, GL_FLOAT,
        GL_FALSE, 5 * sizeof(float), buffer_offset<float>(3u));
}


void quad::update_current(float delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{

}

void quad::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "object_material.diffuse", 0);
	quad_array.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void quad::set_texture(const std::string& path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}

}
