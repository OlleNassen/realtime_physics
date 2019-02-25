#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <array>
#include <vector>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>
#include "shader.hpp"
#include "buffer.hpp"
#include "texture.hpp"


namespace anim
{

using namespace std::literals::chrono_literals;

class joint
{
public:
   auto as_matrix() const{return global_transform * inverse_bind_pose;}

    int parent = 0;
    glm::mat4 local_transform;
    glm::mat4 global_transform;
    glm::mat4 inverse_bind_pose;

private:

};

template <class T>
using skeleton_array = std::array<T, 50>;
using skeleton = skeleton_array<joint>;

struct pose
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
};

struct key_frame
{
    float timepoint = 0.0f;
    skeleton_array<pose> poses;
};

class animation
{
public:
    void load(const std::vector<key_frame>& key_frames);

    void update(float delta, skeleton& joints);

private:
    float time = 0.0f;
    std::vector<key_frame> key_frames;
    std::vector<key_frame>::const_iterator prev;
    std::vector<key_frame>::const_iterator next;

    void update_key_frame();
    void update_pose(skeleton& joints);
};

struct vertex
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec2 texture_coordinate{0.0f, 0.0f};
    glm::vec3 normal{0.0f, 0.0f, 0.0f};
    glm::ivec4 joints{0, 0, 0, 0};
    glm::vec4 weights{0.0f, 0.0f, 0.0f, 0.0f};
};

class model
{
public:
    model();

    void update(float delta);

    void draw(const shader& shader) const;

private:
    texture diffuse{"resources/models/factory_wall_diff_1k.png"};
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    skeleton joints;
    skeleton_array<glm::mat4> world_joints;

    glm::mat4 model_mat{-1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1};

    animation current;

    buffer model_buffer;
    buffer element_buffer{GL_ELEMENT_ARRAY_BUFFER};
    vertex_array model_array;

};

}

#endif // ANIMATION_HPP
