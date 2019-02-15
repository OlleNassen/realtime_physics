#include "box.hpp"
#include <iostream>
#include "objects.hpp"

namespace scene
{

box::box(float x, float y, float z)
    : node(x, y, z)
    , diffuse("resources/images/container2.png")
	, specular("resources/images/container2_specular.png")
{
    box_array.bind();
	box_vbo.data(object::box, GL_STATIC_DRAW);

    box_array.attribute_pointer(0, 3, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), nullptr);
    box_array.attribute_pointer(1, 2, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), buffer_offset<float>(3u));
    box_array.attribute_pointer(2, 3, GL_FLOAT,
        GL_FALSE, 8 * sizeof(float), buffer_offset<float>(5u));
}

void box::update_current(float delta_time,
        const glm::mat4& world_transform, glm::mat4& transform)
{

}

void box::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
    diffuse.uniform(shader, "object_material.diffuse", 0);
    specular.uniform(shader, "object_material.specular", 1);
    shader.uniform("object_material.shininess", 32.0f);
    shader.uniform("model", world_transform);
    box_array.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

}
