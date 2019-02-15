#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include "shader.hpp"

namespace scene
{

using namespace std::literals::chrono_literals;
using milliseconds = std::chrono::milliseconds;

template< typename T>
static constexpr char* buffer_offset(unsigned int offset)
{
    return reinterpret_cast<char*>(offset * sizeof(T));
}

class node
{
public:
    node() = default;
    virtual ~node() = default;

    node(float x, float y, float z);

    void attach_child(node* child);
    void clear();

    void sort(glm::vec3& pos);
    void update(milliseconds delta);
    void prepare_render(const shader& shader) const;
    void render(const shader& shader) const;

	float distance_to(const glm::vec3& other) const;
private:
    std::vector<node*> children;
    glm::mat4 local{1.0f};
    glm::mat4 world{1.0f};

    void update(milliseconds delta, const glm::mat4& world_transform);

    virtual void update_current(milliseconds delta,
        const glm::mat4& world_transform, glm::mat4& transform);
    virtual void prepare_render_current(const shader& shader,
        const glm::mat4& world_transform) const;
    virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const;

    void sort_children(glm::vec3& pos);
};

}

#endif // SCENE_NODE_HPP
