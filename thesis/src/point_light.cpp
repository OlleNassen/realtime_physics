#include "point_light.hpp"

namespace scene
{

point_light::point_light()
{
	ambient = glm::vec3(0.4, 0.4, 0.4);
	diffuse = glm::vec3(1, 1, 0.1);
	specular = glm::vec3(0.1, 1, 0.1);
	da_box = new box(5);
	attach_child(da_box);
}

void point_light::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
}

void point_light::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
        shader.uniform("point_lights[0].position",
            glm::vec3(world_transform * glm::vec4(1.0f)));
        shader.uniform("point_lights[0].ambient", ambient);
        shader.uniform("point_lights[0].diffuse", diffuse);
        shader.uniform("point_lights[0].specular", specular);
        shader.uniform("point_lights[0].constant", 1.0f);
        shader.uniform("point_lights[0].linear", 0.09f);
        shader.uniform("point_lights[0].quadratic", 0.032f);

}

}
