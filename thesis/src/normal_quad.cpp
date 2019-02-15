#include "normal_quad.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{

normal_quad::normal_quad(float x, float y, float z)
	: node(x, y, z)
	, quad_texture(new texture("resources/images/edvard.png"))
{
	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1;
	glm::vec3 tangent2;
	// triangle 1
	// ----------
	auto edge1 = pos2 - pos1;
	auto edge2 = pos3 - pos1;
	auto deltaUV1 = uv2 - uv1;
	auto deltaUV2 = uv3 - uv1;

	auto f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);

	float quad_vertices[] = {
		// positions  // normal // texcoords  // tangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
		tangent1.x, tangent1.y, tangent1.z,

		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
		tangent1.x, tangent1.y, tangent1.z,

		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
		tangent1.x, tangent1.y, tangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
		tangent2.x, tangent2.y, tangent2.z,

		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
		tangent2.x, tangent2.y, tangent2.z,

		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y,
		tangent2.x, tangent2.y, tangent2.z,
	};

	auto stride = 11 * sizeof(float);

	quad_array.bind();
	quad_vbo.data(sizeof(quad_vertices), &quad_vertices[0], GL_STATIC_DRAW);
	quad_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	auto offset = 3u;
	quad_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, stride,
        buffer_offset<float>(offset));
	offset += 3u;
	quad_array.attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, stride,
        buffer_offset<float>(offset));
	offset += 2u;
	quad_array.attribute_pointer(3, 3, GL_FLOAT, GL_FALSE, stride,
        buffer_offset<float>(offset));

	quad_normal = new texture("resources/images/brickwall_normal.jpg",
        wrap::REPEAT, filter::LINEAR, format::RGB);
}

normal_quad::~normal_quad()
{
	if (quad_texture)
		delete quad_texture;
	if (quad_normal)
		delete quad_normal;
}

void normal_quad::update_current(milliseconds delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
	transform = glm::scale(transform, glm::vec3(0.1, 0.1, 1));
}

void normal_quad::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", world_transform);
	quad_texture->uniform(shader, "diffuse_map", 0);
	quad_normal->uniform(shader, "normal_map", 1);
	quad_array.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void normal_quad::set_texture(const std::string& path)
{
	delete quad_texture;
	quad_texture = new texture(path, wrap::REPEAT, filter::LINEAR, format::RGB);
}

}
