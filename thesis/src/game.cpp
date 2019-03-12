#include "game.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::ostream& operator<<(std::ostream& os, const glm::mat4& m)
{
    using std::endl;
    constexpr char c = ' ';
    return os
        << m[0][0] << c << m[1][0] << c << m[2][0] << c << m[3][0] << endl
        << m[0][1] << c << m[1][1] << c << m[2][1] << c << m[3][1] << endl
        << m[0][2] << c << m[1][2] << c << m[2][2] << c << m[3][2] << endl
        << m[0][3] << c << m[1][3] << c << m[2][3] << c << m[3][3] << endl;
}

void renderSphere();

game::game()
	: game_camera(glm::radians(45.0f), width, height, 0.1f, 10000.0f)
	, light(glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f))
{
	srand(time(NULL));
	assign_window(game_window);

	assign_mouse_callback(
        std::bind(
        &camera::on_mouse_moved,
        &game_camera,
        std::placeholders::_1,
        std::placeholders::_2));

    bind_key("up", key::W);
    bind_key("down", key::S);
    bind_key("left", key::A);
    bind_key("right", key::D);
    bind_key("shift", key::Q);
    bind_key("escape", key::ESCAPE);

    assign_key_callback("escape",
        std::bind(&window::on_escape, &game_window),
        nullptr);

    assign_key_callback("up",
        std::bind(&camera::up_pressed, &game_camera),
        std::bind(&camera::up_released, &game_camera));

    assign_key_callback("down",
        std::bind(&camera::down_pressed, &game_camera),
        std::bind(&camera::down_released, &game_camera));

    assign_key_callback("left",
        std::bind(&camera::left_pressed, &game_camera),
        std::bind(&camera::left_released, &game_camera));

    assign_key_callback("right",
        std::bind(&camera::right_pressed, &game_camera),
        std::bind(&camera::right_released, &game_camera));

    assign_key_callback("shift",
        std::bind(&camera::fast_pressed, &game_camera),
        std::bind(&camera::fast_released, &game_camera));

	light_pos = glm::vec3(50, 5, -15);
    phong_pos = glm::vec3(0, 10, 5);

    terrain.update(0.0f);

	ThirdPersonCamera.First = true;
	ThirdPersonCamera.CameraPosition.Radius = 5.0f;

	physics_world.player_position.old_position.x = 90.f;
	physics_world.player_position.old_position.y = 70.0f;
	physics_world.player_position.old_position.z = 35.0f;

	physics_world.player_position.position.x = 90.f;
	physics_world.player_position.position.y = 70.0f;
	physics_world.player_position.position.z = 35.0f;

	physics_world.player_position.old_rotation.x = 0.f;
	physics_world.player_position.old_rotation.y = 0.0f;
	physics_world.player_position.old_rotation.z = 0.0f;

	physics_world.player_position.rotation.x = 0.f;
	physics_world.player_position.rotation.y = 0.0f;
	physics_world.player_position.rotation.z = 0.0f;
	
	physics_world.player_collider.radius = 1.0f;

	const int width = 256;
	for (unsigned int i = 0; i < terrain.vertices.size() - (width + 1); i++)
	{
		if (i % width != width - 1)
		{
			triangle a;
			a.x = terrain.vertices[i].position;
			a.y = terrain.vertices[i + width].position;
			a.z = terrain.vertices[i + 1].position;

			triangle b;
			b.x = terrain.vertices[i + width].position;
			b.y = terrain.vertices[i + width + 1].position;
			b.z = terrain.vertices[i + 1].position;

			physics_world.triangles.emplace_back(a);
			physics_world.triangles.emplace_back(b);
		}

	}

}

void game::run()
{	
	double timestep = 1.0 / 120.0;
	double last_time = glfwGetTime();
	double delta_time = 0.0;

	physics_world.dt = (float)timestep;

	while (game_window.is_open())
	{
		double now = glfwGetTime();
		delta_time += now - last_time;
        last_time = now;

		while (delta_time > timestep)
		{
			game_window.poll_events();
			update((float)timestep);
			delta_time -= timestep;
		}

		render();
		
	}
}

void game::render()
{
	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);

	//triangle_shader.use();
	//BindCamera(&ThirdPersonCamera, &triangle_shader);
	//draw(&physics_world, triangle_shader);

	static glm::vec3 dir_light_dir = glm::vec3(0, -1, 0);
	static glm::vec3 dir_light_color = glm::vec3(17, 17, 53);
	static float dir_light_intensity = 100.f;

	terrain_shader.use();
	BindCamera(&ThirdPersonCamera, &terrain_shader);
	terrain_shader.uniform("lightPositions[0]", physics_world.player_position.position + glm::vec3(0, 2, 0));
	terrain_shader.uniform("lightColors[0]", glm::vec3(1, 1, 1));
	temp_model.TexSteal(skybox_shader);
	terrain_shader.uniform("dir_light_dir", dir_light_dir);
	terrain_shader.uniform("dir_light_color", dir_light_color);
	terrain_shader.uniform("dir_light_intensity", dir_light_intensity);
	terrain.render(terrain_shader);

	skybox_shader.use();
	BindCamera(&ThirdPersonCamera, &skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(glm::mat3(game_camera.get_view())));
	sky.render(skybox_shader);

	//Animated model
	anim.use();
	BindCamera(&ThirdPersonCamera, &anim);
	anim.uniform("lightPositions[0]", physics_world.player_position.position + glm::vec3(0,2,0));
	anim.uniform("lightColors[0]", glm::vec3(1,1,1));

	anim.uniform("dir_light_dir", dir_light_dir);
	anim.uniform("dir_light_color", dir_light_color);
	anim.uniform("dir_light_intensity", dir_light_intensity);

	temp_model.draw(anim);
	renderSphere();

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	update_verlet(&physics_world);	
	temp_model.set_position(physics_world.player_position.position);
	
	double MX, MY;
	glfwGetCursorPos(game_window.glfw_window, &MX, &MY);
	ThirdPersonCamera.PlayerPosition = physics_world.player_position.position;

	UpdateCamera(&ThirdPersonCamera, (int)MX, (int)MY,
		(bool)glfwGetKey(game_window.glfw_window, GLFW_KEY_W),
		(bool)glfwGetKey(game_window.glfw_window, GLFW_KEY_A),
		(bool)glfwGetKey(game_window.glfw_window, GLFW_KEY_S),
		(bool)glfwGetKey(game_window.glfw_window, GLFW_KEY_D));

	physics_world.player_position.position = ThirdPersonCamera.PlayerPosition;

	if (physics_world.model_mat != temp_model.model_mat)
	{
		temp_model.model_mat *= physics_world.model_mat;
	}

}


void renderSphere()
{
	static unsigned int sphereVAO = 0;
	static unsigned int indexCount;
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}