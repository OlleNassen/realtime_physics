#include "directional_light.hpp"
#include <GL/glew.h>

directional_light::directional_light(
    const glm::vec3& direction,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
    : direction(direction)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
{

}

void directional_light::bind(const shader& shader)
{
    shader.uniform("light.direction", direction);
    shader.uniform("light.ambient", ambient);
    shader.uniform("light.diffuse", diffuse);
    shader.uniform("light.specular", specular);
}
