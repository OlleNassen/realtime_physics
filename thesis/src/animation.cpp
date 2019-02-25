#include "animation.hpp"
#include <iostream>
#include <algorithm>
#include "game.hpp"

namespace anim
{

constexpr glm::mat4 ai_to_glm(const aiMatrix4x4& mat)
{
    return
    {
        mat.a1, mat.b1, mat.c1, mat.d1,
        mat.a2, mat.b2, mat.c2, mat.d2,
        mat.a3, mat.b3, mat.c3, mat.d3,
        mat.a4, mat.b4, mat.c4, mat.d4
    };
}

constexpr float calculate_progression(
    float prev, float next, float time)
{
    auto total_time = next - prev;
    auto current_time = time - prev;
    return current_time / total_time;
}

pose mix(const pose& x, const pose& y, float a)
{
    return
    {
        glm::mix(x.position, y.position, a),
        glm::slerp(x.rotation, y.rotation, a)
    };
}

void load_mesh(const aiMesh* mesh, std::vector<vertex>& vertices,
    std::vector<unsigned int>& indices)
{
	vertices.resize(mesh->mNumVertices);

	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		vertices[i].position.x = mesh->mVertices[i].x;
		vertices[i].position.y = mesh->mVertices[i].y;
		vertices[i].position.z = mesh->mVertices[i].z;

		vertices[i].texture_coordinate.x = mesh->mTextureCoords[0][i].x;
		vertices[i].texture_coordinate.y = mesh->mTextureCoords[0][i].y;

		vertices[i].normal.x = mesh->mNormals[i].x;
		vertices[i].normal.y = mesh->mNormals[i].y;
		vertices[i].normal.z = mesh->mNormals[i].z;
	}

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		auto face = mesh->mFaces[i];
		for (auto j = 0u; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (auto i = 0u; i < mesh->mNumBones; ++i)
	{
		auto* bone = mesh->mBones[i];

		for (auto j = 0u; j < bone->mNumWeights; ++j)
		{
            auto& weight = bone->mWeights[j];

            if (vertices[weight.mVertexId].weights.x == 0.0f)
            {
                vertices[weight.mVertexId].joints.x = i+1;
                vertices[weight.mVertexId].weights.x = weight.mWeight;
            }
            else if (vertices[weight.mVertexId].weights.y == 0.0f)
            {
                vertices[weight.mVertexId].joints.y = i+1;
                vertices[weight.mVertexId].weights.y = weight.mWeight;
            }
            else if (vertices[weight.mVertexId].weights.z == 0.0f)
            {
                vertices[weight.mVertexId].joints.z = i+1;
                vertices[weight.mVertexId].weights.z = weight.mWeight;
            }
            else if (vertices[weight.mVertexId].weights.w == 0.0f)
            {
                vertices[weight.mVertexId].joints.w = i+1;
                vertices[weight.mVertexId].weights.w = weight.mWeight;
            }
		}
	}
}

void load_parent_names(const aiNode& node, std::vector<std::string>& names)
{
    if (strcmp(node.mName.C_Str(), "Camera"))
    {
        names.emplace_back(node.mName.C_Str());
    }

    for (auto i = 0u; i < node.mNumChildren; ++i)
	{
        load_parent_names(*node.mChildren[i], names);
	}
}

void load_parent_indices(const aiNode& node,
    const std::vector<std::string>& names,
    int& index, skeleton& joints)
{
    if(strcmp(node.mName.C_Str(), "Camera"))
    {
        auto& joint = joints[index];
        for (auto i = 0u; i < names.size(); ++i)
        {
            if(names[i].compare(node.mParent->mName.C_Str()) == 0)
            {
                joint.parent = i;
                joint.local_transform = ai_to_glm(node.mTransformation);
                joint.global_transform = joints[i].global_transform
                    * joint.local_transform;
                joint.inverse_bind_pose = glm::inverse(joint.global_transform);
            }
        }
        if(&joint == &joints.front())
        {
            joint.local_transform = ai_to_glm(node.mTransformation);
            joint.global_transform = joint.local_transform;
            joint.inverse_bind_pose = glm::inverse(joint.global_transform);
        }

        ++index;
    }

    for (auto i = 0u; i < node.mNumChildren; ++i)
    {
        load_parent_indices(*node.mChildren[i], names, index, joints);
    }
}

void load_skeleton(const aiNode* node, skeleton& joints)
{
    std::vector<std::string> names;
    for (auto i = 0u; i < node->mNumChildren; ++i)
	{
        load_parent_names(*node->mChildren[i], names);
	}

	auto index = 0;
	for (auto i = 0u; i < node->mNumChildren; ++i)
    {
        load_parent_indices(*node->mChildren[i], names, index, joints);
    }
}

void load_key_frames(const aiAnimation* anim,
    std::vector<key_frame>& key_frames)
{
	key_frames.resize(anim->mChannels[0]->mNumPositionKeys);

	for (auto i = 0u; i < anim->mNumChannels; ++i)
	{
        auto* channel = anim->mChannels[i];
        for (auto j = 0u; j < channel->mNumPositionKeys; ++j)
		{
            if (i == 0u)
            {
                using namespace std::chrono;
                key_frames[j].timepoint =
                    (float)(channel->mPositionKeys[j].mTime);
            }

            aiVector3D v = channel->mPositionKeys[j].mValue;
            aiQuaternion q = channel->mRotationKeys[j].mValue;

            key_frames[j].poses[i+1].position = glm::vec3{v.x, v.y, v.z};
            key_frames[j].poses[i+1].rotation = glm::quat{q.w, q.x, q.y, q.z};
		}
	}
}

void import_model(const std::string& path,
	std::vector<vertex>& vertices,
	std::vector<unsigned int>& indices,
	skeleton& joints,
	std::vector<key_frame>& key_frames)
{
	Assimp::Importer importer;
	auto* scene = importer.ReadFile(path.c_str(),
		aiProcess_Triangulate |
        aiProcess_FlipUVs);

    load_mesh(scene->mMeshes[0], vertices, indices);
	//load_skeleton(scene->mRootNode, joints);
	//load_key_frames(scene->mAnimations[0], key_frames);
}




void animation::load(const std::vector<key_frame>& key_frames)
{
    this->key_frames = key_frames;
    prev = this->key_frames.begin();
    next = this->key_frames.begin();
    ++next;
}

void animation::update(float delta, skeleton& joints)
{
    time += delta;

    //update_key_frame();
    //update_pose(joints);
}

void animation::update_key_frame()
{
    if (time > next->timepoint)
    {
        ++prev;
        ++next;

        if (next == key_frames.end())
        {
            time = 0.0f;
            prev = key_frames.begin();
            next = key_frames.begin();
            ++next;
        }
    }
}

void animation::update_pose(skeleton& joints)
{
    auto progression = calculate_progression(
        prev->timepoint, next->timepoint, time);

    auto new_transform = [&](int i) -> glm::mat4
    {
        auto p = mix(prev->poses[i], next->poses[i], progression);
        return glm::translate(glm::mat4{1.0f}, p.position)
            * glm::mat4_cast(p.rotation);

    };

    joints.front().local_transform = new_transform(0);
    joints.front().global_transform = joints.front().local_transform;
    for (auto i = 1u; i < joints.size(); ++i)
    {
        auto& j = joints[i];
        j.local_transform =  new_transform(i);
        j.global_transform =
            joints[j.parent].global_transform * j.local_transform;
    }
}

model::model()
{
    std::vector<key_frame> key_frames;

	import_model("resources/models/untitled.dae",
        vertices, indices, joints,
        key_frames);

	//current.load(key_frames);

	auto stride = 12 * sizeof(float) + 4 * sizeof(unsigned int);

	model_array.bind();
	model_buffer.data(vertices, GL_STATIC_DRAW);
	element_buffer.data(indices, GL_STATIC_DRAW);

	model_array.attribute_pointer(0, 3,
        GL_FLOAT, GL_FALSE, stride, (void*)0);
	model_array.attribute_pointer(1, 2,
        GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	model_array.attribute_pointer(2, 3,
        GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

	model_array.attribute_pointer(3, 4,
        GL_UNSIGNED_INT, stride, (void*)(8 * sizeof(float)));
	model_array.attribute_pointer(4, 4,
        GL_FLOAT, GL_FALSE, stride,
        (void*)(8 * sizeof(float) + 4 * sizeof(unsigned int)));

	world_joints.fill(glm::mat4(1.0f));
}

void model::update(float delta)
{
	current.update(delta, joints);

	std::transform(joints.begin(), joints.end(), world_joints.begin(),
        std::mem_fn(&joint::as_matrix));
}

void model::draw(const shader& shader) const
{
	shader.uniform("model", model_mat);
	shader.uniform("joint_transforms", world_joints);
    diffuse.uniform(shader, "diffuse", 0);
	model_array.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
