#ifndef BOX_HPP
#define BOX_HPP

#include "node.hpp"
#include "buffer.hpp"

#include "texture.hpp"

namespace scene
{

class box : public node
{
public:
    box(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void draw()
	{
		box_array.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
private:
	vertex_array box_array;
	buffer box_vbo;
	texture diffuse;
	texture specular;

    void update_current(milliseconds delta_time,
        const glm::mat4& world_transform, glm::mat4& transform) override;

    void render_current(const shader& shader,
        const glm::mat4& world_transform) const override;


};

}

#endif // BOX_HPP
