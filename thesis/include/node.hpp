#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include "shader.hpp"

namespace scene
{

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

    void update(float delta);
    void render(const shader& shader) const;

private:
    std::vector<node*> children;
    glm::mat4 local{1.0f};
    glm::mat4 world{1.0f};

    void update(float delta, const glm::mat4& world_transform);

    virtual void update_current(float delta,
        const glm::mat4& world_transform, glm::mat4& transform);

	virtual void render_current(const shader& shader,
        const glm::mat4& world_transform) const;
};

}

#endif // SCENE_NODE_HPP
