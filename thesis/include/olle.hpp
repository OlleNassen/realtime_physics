#ifndef OLLE_HPP
#define OLLE_HPP
#include <array>
#include "buffer.hpp"
#include "node.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "box.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include <iostream>

/*
//TEMP RENDER:
for (auto i = root->get_child_iterator_start(); i < root->get_child_iterator_end(); ++i)
{
//Should be nothing on this level
//(*i)->draw(basic_shader);
for (auto k = (*i)->get_child_iterator_start(); k < (*i)->get_child_iterator_end(); ++k)
{
glm::mat4 scale = glm::mat4(1.f);
scale[0][0] = (*k)->get_model_scale().x;
scale[1][1] = (*k)->get_model_scale().y;
scale[2][2] = (*k)->get_model_scale().z;
glm::mat4 model_matrix = (*k)->get_world_transform() * scale;

basic_shader.uniform("model", model_matrix);
(*k)->draw(basic_shader);

for (auto lol = (*k)->get_child_iterator_start(); lol < (*k)->get_child_iterator_end(); ++lol)
{
glm::mat4 scale = glm::mat4(1.f);
scale[0][0] = (*lol)->get_model_scale().x;
scale[1][1] = (*lol)->get_model_scale().y;
scale[2][2] = (*lol)->get_model_scale().z;
glm::mat4 model_matrix = (*lol)->get_world_transform() * scale;

basic_shader.uniform("model", model_matrix);
(*lol)->draw(basic_shader);
}
}
}

//TEMP UPDATE:
root->update(0.016f);

*/

class Node
{
public:
	Node(mesh* mesh_ptr = nullptr, glm::vec4 color = glm::vec4(1,1,0,1))
	{
		this->mesh_ptr = mesh_ptr;
		parent = nullptr;
		this->color = color;
		transform = glm::mat4(1.f);
		world_transform = glm::mat4(1.f);
		model_scale = glm::vec4(1.f);
		color = glm::vec4(1.f);
	}
	virtual ~Node()
	{
		for (auto i = 0u; i < children.size(); i++)
			delete children[i];
	}

	glm::mat4 get_world_transform()const { return world_transform; }

	void set_transform(const glm::mat4& matrix) { this->transform = matrix; }
	glm::mat4 get_transform()const { return this->transform; }

	void set_model_scale(const glm::vec3 scale) { this->model_scale = scale; }
	glm::vec3 get_model_scale()const { return this->model_scale; }

	void set_color(const glm::vec4 color) { this->color = color; }
	glm::vec4 get_color()const { return this->color; }

	void set_mesh(mesh* ptr){mesh_ptr = ptr;}
	mesh* get_mesh()const{return mesh_ptr;}

	void add_child(Node* child)
	{
		child->parent = this;
		children.emplace_back(child);
	}

	virtual void update(float delta_time)
	{
		if (parent)
		{
			world_transform = parent->world_transform * transform;
		}
		else
		{
			world_transform = transform;
		}

		for (auto& child : children)
		{
			child->update(delta_time);
		}
	}

	virtual void draw(const shader& shade)
	{
		if (mesh_ptr)
		{
			mesh_ptr->draw(shade);
		}
	}

	std::vector <Node*>::const_iterator  get_child_iterator_start()
	{
		return  children.begin();
	}
	std::vector <Node*>::const_iterator  get_child_iterator_end()
	{
		return  children.end();
	}

protected:
	Node* parent;
	mesh* mesh_ptr;
	glm::mat4 world_transform;
	glm::mat4 transform;
	glm::vec3 model_scale;
	glm::vec4 color;
	std::vector<Node*> children;
};

class cube_robot : public Node
{
protected:
	mesh* cube;
	Node* body;
	Node* head;
	Node* left_arm;
	Node* right_arm;
public:
	cube_robot()
	{
		create_cube();
		set_mesh(cube);
		//BODY
		body = new Node(cube, glm::vec4(1, 0, 0, 1));    //Red!
		body->set_model_scale(glm::vec3(10, 15, 5));
		glm::mat4 tran(1.f);
		//Transform
		tran[3][0] = 0.f;
		tran[3][1] = 35.f;
		tran[3][2] = 0.f;

		body->set_transform(tran);
		add_child(body);

		//HEAD
		head = new Node(cube, glm::vec4(0, 1, 0, 1));    //Green!
		head->set_model_scale(glm::vec3(5, 5, 5));
		glm::mat4 tran1(1.f);
		tran1[3][0] = 0.f;
		tran1[3][1] = 30.f;
		tran1[3][2] = 0.f;

		head->set_transform(tran1);
		body->add_child(head);

		//LEFT ARM
		left_arm = new Node(cube, glm::vec4(0, 0, 1, 1));    //Blue!
		left_arm->set_model_scale(glm::vec3(3, -18, 3));
		glm::mat4 tran2(1.f);
		tran2[3][0] = -12.f;
		tran2[3][1] = 30.f;
		tran2[3][2] = -1.f;

		left_arm->set_transform(tran2);
		body->add_child(left_arm);

		//RIGHT ARM
		right_arm = new Node(cube, glm::vec4(0, 0, 1, 1));    //Blue!
		right_arm->set_model_scale(glm::vec3(3, -18, 3));
		glm::mat4 tran3(1.f);
		tran3[3][0] = 12.f;
		tran3[3][1] = 30.f;
		tran3[3][2] = -1.f;

		right_arm->set_transform(tran3);
		body->add_child(right_arm);

		//LEFT LEG
		Node* left_leg = new Node(cube, glm::vec4(0, 1, 1, 1));    //Red!
		left_leg->set_model_scale(glm::vec3(3, 17.5, 3));
		glm::mat4 tran4(1.f);
		tran4[3][0] = -8.f;
		tran4[3][1] = 0.f;
		tran4[3][2] = 0.f;

		left_leg->set_transform(tran4);
		body->add_child(left_leg);

		//RIGHT LEG
		Node* right_leg = new Node(cube, glm::vec4(0, 1, 1, 1));    //Red!
		right_leg->set_model_scale(glm::vec3(3, 17.5, 3));
		glm::mat4 tran5(1.f);
		tran5[3][0] = 8.f;
		tran5[3][1] = 0.f;
		tran5[3][2] = 0.f;

		right_leg->set_transform(tran5);
		body->add_child(right_leg);

	}
	~cube_robot()
	{

	}
	void create_cube()
	{
		std::vector<vertex>vertices =
		{
			//pos, uv, normal
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, -1.0f)),
			vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)),
			vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)),
			vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f),   glm::vec3(0.0f,  0.0f, -1.0f)),
			vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f),   glm::vec3(0.0f,  0.0f, -1.0f)),
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, -1.0f)),

			vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
			vertex(glm::vec3(0.5f, -0.5f,   0.5f), glm::vec2(1.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
			vertex(glm::vec3(0.5f,  0.5f,   0.5f), glm::vec2(1.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
			vertex(glm::vec3(0.5f,  0.5f,   0.5f), glm::vec2(1.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
			vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
			vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f,  0.0f, 1.0f)),
				  
			vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
				
			vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
			vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f)),
				
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f,  -0.5f), glm::vec2(1.0f, 1.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f,   0.5f), glm::vec2(1.0f, 0.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			vertex(glm::vec3(0.5f, -0.5f,   0.5f), glm::vec2(1.0f, 0.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
			vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
				 
			vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f),  glm::vec3(0.0f,  1.0f,  0.0f)),
			vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f),  glm::vec3(0.0f,  1.0f,  0.0f)),
			vertex(glm::vec3(0.5f,  0.5f,   0.5f), glm::vec2(1.0f, 0.0f),  glm::vec3(0.0f,  1.0f,  0.0f)),
			vertex(glm::vec3(0.5f,  0.5f,   0.5f), glm::vec2(1.0f, 0.0f),  glm::vec3(0.0f,  1.0f,  0.0f)),
			vertex(glm::vec3(0.5f,  0.5f,  -0.5f), glm::vec2(1.0f, 1.0f),  glm::vec3(0.0f,  1.0f,  0.0f)),
			vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f),  glm::vec3(0.0f,  1.0f,  0.0f))
		};
		std::vector<unsigned int> throw_away1;
		std::vector<texture>throw_away2;
		cube = new mesh(vertices, throw_away1, throw_away2);
	}
	virtual void update(float delta_time)override
	{
		transform = 
			glm::rotate(transform, delta_time / 10.0f,
				glm::vec3(0.f, 1.f, 0.f));

		head->set_transform(
			glm::rotate(head->get_transform(), -delta_time / 10.0f,
				glm::vec3(0.f, 1.f, 0.f)));

		left_arm->set_transform(
			glm::rotate(left_arm->get_transform(), -delta_time / 10.0f,
				glm::vec3(1.f, 0.f, 0.f)));

		right_arm->set_transform(
			glm::rotate(right_arm->get_transform(), delta_time / 10.0f,
				glm::vec3(1.f, 0.f, 0.f)));

		Node::update(delta_time);
	}
};

#endif
