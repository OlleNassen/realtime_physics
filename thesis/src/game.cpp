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

struct renderable_spheres
{
	static const int num_spheres = 2;
	glm::mat4 model[num_spheres];
	unsigned int vao[num_spheres];
	unsigned int vbo[num_spheres];
	unsigned int ebo[num_spheres];
	unsigned int index_count[num_spheres];
};

void init_spheres(renderable_spheres* spheres);
void draw_spheres(const renderable_spheres* spheres, int type[], const shader& shader);

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

    terrain.update(0.0f);

	ThirdPersonCamera.First = true;
	ThirdPersonCamera.CameraPosition.Radius = 5.0f;

	physics_world.player_position.old_position.x = 210.f;
	physics_world.player_position.old_position.y = 55.f;
	physics_world.player_position.old_position.z = 210.f;

	physics_world.player_position.position.x = 210.f;
	physics_world.player_position.position.y = 55.f;
	physics_world.player_position.position.z = 210.f;
	
	physics_world.player_collider.radius = 1.0f;
	physics_world.player_collider.weight = 32672.f;
	physics_world.player_collider.elasticity = 0.6f;

	physics_world.enemy_position.old_position.x = 200.f;
	physics_world.enemy_position.old_position.y = 50.f;
	physics_world.enemy_position.old_position.z = 200.f;

	physics_world.enemy_position.position.x = 200.f;
	physics_world.enemy_position.position.y = 50.f;
	physics_world.enemy_position.position.z = 200.f;

	physics_world.enemy_collider.radius = 1.0f;
	physics_world.enemy_collider.weight = 11728.f;
	physics_world.enemy_collider.elasticity = 0.21f;

	physics_world.player_position.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	physics_world.player_position.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	physics_world.enemy_position.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	physics_world.enemy_position.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	physics_world.gravity = glm::vec3(0.0f, -9.82f, 0.0f);

	ball_material[0] = 0;
	ball_material[1] = 1;

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

	static glm::vec3 dir_light_dir = glm::vec3(1, -1, 0);
	static glm::vec3 dir_light_color = glm::vec3(1, 1, 1);
	static float dir_light_intensity = 1.f;

	terrain_shader.use();
	BindCamera(&ThirdPersonCamera, &terrain_shader);
	switch (terrain_type)
	{
	case 0: terrain.rock(terrain_shader); break;
	case 1: terrain.snow(terrain_shader); break;
	case 2: terrain.wood(terrain_shader); break;
	}
	terrain_shader.uniform("dir_light_dir", dir_light_dir);
	terrain_shader.uniform("dir_light_color", dir_light_color);
	terrain_shader.uniform("dir_light_intensity", dir_light_intensity);
	terrain.render(terrain_shader);

	skybox_shader.use();
	BindCamera(&ThirdPersonCamera, &skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(glm::mat3(ThirdPersonCamera.View)));
	sky.render(skybox_shader);

	//Animated model
	anim.use();
	BindCamera(&ThirdPersonCamera, &anim);

	anim.uniform("dir_light_dir", dir_light_dir);
	anim.uniform("dir_light_color", dir_light_color);
	anim.uniform("dir_light_intensity", dir_light_intensity);

	static renderable_spheres spheres;
	static bool run_once = false;
	if (!run_once)
	{
		init_spheres(&spheres);
		run_once = true;
	}
	spheres.model[0][3] = glm::vec4(physics_world.player_position.old_position, 1);
	spheres.model[1][3] = glm::vec4(physics_world.enemy_position.old_position, 1);

	draw_spheres(&spheres, ball_material ,anim);

	glDisable(GL_DEPTH_TEST);
	text_shader.use();
	text_shader.uniform("projection", glm::ortho(0.0f, 1280.f, 0.0f, 720.f));
	text_shader.uniform("text_color", glm::vec3(17.f / 255.f, 17.f / 255.f, 53 / 255.f));

	if (is_verlet)
	{
		temp_text.render_text("VERLET", 0, 0, 1.f);
	}
	else
	{
		temp_text.render_text("EULER", 0, 0, 1.f);
	}

	
	int vx = (int)physics_world.player_position.velocity.x;
	int vy = (int)physics_world.player_position.velocity.y;
	int vz = (int)physics_world.player_position.velocity.z;
	temp_text.render_text("Velocity: " + std::to_string(vx) + ", " + std::to_string(vy) + ", " + std::to_string(vz), 0, 700.f, .3f);

	temp_text.render_text("Player weight: " + std::to_string(physics_world.player_collider.weight).substr(0, 7) + "kg", 500.f, 700.f, .3f);

	temp_text.render_text("Friend weight: " + std::to_string(physics_world.enemy_collider.weight).substr(0, 7) + "kg", 900.f, 700.f, .3f);

	temp_text.render_text("Player elasticity: " + std::to_string(physics_world.player_collider.elasticity).substr(0,4), 350, 25.f, .3f);
	temp_text.render_text("Friend elasticity: " + std::to_string(physics_world.enemy_collider.elasticity).substr(0, 4), 350.f, 5.f, .3f);

	temp_text.render_text("Material friction: " + std::to_string(physics_world.friction_val).substr(0, 4), 900.f, 0.f, .3f);

	glEnable(GL_DEPTH_TEST);

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_Q))
	{
		physics_world.enemy_position.old_position.x = 200.f;
		physics_world.enemy_position.old_position.y = 50.f;
		physics_world.enemy_position.old_position.z = 200.f;

		physics_world.enemy_position.position.x = 200.f;
		physics_world.enemy_position.position.y = 50.f;
		physics_world.enemy_position.position.z = 200.f;
	}
	
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F1))
	{
		terrain_type = 0;
		physics_world.friction_val = 0.2f;
	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F2))
	{
		terrain_type = 1;
		physics_world.friction_val = 0.02f; // ICE
	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F3))
	{
		terrain_type = 2;
		physics_world.friction_val = 0.35f; // sand

	}

	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F5))
	{
		ball_material[0] = 0;
		physics_world.player_collider.weight = 32672.f;
		physics_world.player_collider.elasticity = 0.6f;
	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F6))
	{
		ball_material[0] = 1;
		physics_world.player_collider.weight = 11728.f;
		physics_world.player_collider.elasticity = 0.21f;
	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_F7))
	{
		ball_material[0] = 2;
		physics_world.player_collider.weight = 6375.f;
		physics_world.player_collider.elasticity = 0.9f;
	}

	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_5))
	{
		ball_material[1] = 0;
		physics_world.enemy_collider.weight = 32672.f;
		physics_world.enemy_collider.elasticity = 0.6f;

	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_6))
	{
		ball_material[1] = 1;
		physics_world.enemy_collider.weight = 11728.f;
		physics_world.enemy_collider.elasticity = 0.21f;
	}
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_7))
	{
		ball_material[1] = 2;
		physics_world.enemy_collider.weight = 6375.f;
		physics_world.enemy_collider.elasticity = 0.9f;
	}

	
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_1)) is_verlet = true;
	if (glfwGetKey(game_window.glfw_window, GLFW_KEY_2)) is_verlet = false;
	
	if (is_verlet)
	{
		update_verlet(&physics_world);
	}
	else
	{
		update_euler(&physics_world);
	}

	double MX, MY;
	glfwGetCursorPos(game_window.glfw_window, &MX, &MY);
	ThirdPersonCamera.PlayerPosition = physics_world.player_position.position;

	UpdateCamera(&ThirdPersonCamera, (int)MX, (int)MY, delta_time);

	if (glfwGetMouseButton(game_window.glfw_window, GLFW_MOUSE_BUTTON_LEFT))
	{
		physics_world.player_position.acceleration 
			= 100.0f * DirectionVector(ThirdPersonCamera.CameraPosition);
	}
	else
	{
		physics_world.player_position.acceleration = glm::vec3(0.0f);
	}
}

void init_spheres(renderable_spheres* spheres)
{
	for (int i = 0; i < spheres->num_spheres; i++)
	{
		glGenVertexArrays(1, &spheres->vao[i]);

		glGenBuffers(1, &spheres->vbo[i]);
		glGenBuffers(1, &spheres->ebo[i]);

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
		spheres->index_count[i] = indices.size();

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
		glBindVertexArray(spheres->vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, spheres->vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spheres->ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

		spheres->model[i] = glm::mat4(1.0f);
		spheres->model[i][2][2] = -1.0f;
	}


}

void draw_spheres(const renderable_spheres* spheres, int type[], const shader& shader)
{
	static texture albedo{ "resources/textures/rustediron2_albedo.png" };
	static texture normal{ "resources/textures/rustediron2_normal.png" };
	static texture metallic{ "resources/textures/rustediron2_metallic.png" };
	static texture roughness{ "resources/textures/rustediron2_roughness.png" };

	static texture sand_albedo{ "resources/textures/sandstone/sandstonecliff-albedo.png" };
	static texture sand_normal{ "resources/textures/sandstone/sandstonecliff-normal-ue.png" };
	static texture sand_metallic{ "resources/textures/sandstone/sandstonecliff-metalness.png" };
	static texture sand_roughness{ "resources/textures/sandstone/sandstonecliff-roughness.png" };

	static texture bamboo_albedo{ "resources/textures/bamboo/bamboo-wood-semigloss-albedo.png" };
	static texture bamboo_normal{ "resources/textures/bamboo/bamboo-wood-semigloss-normal.png" };
	static texture bamboo_metallic{ "resources/textures/bamboo/bamboo-wood-semigloss-metal.png" };
	static texture bamboo_roughness{ "resources/textures/bamboo/bamboo-wood-semigloss-roughness.png" };

	for (int i = 0; i < spheres->num_spheres; i++)
	{
		if (type[i] == 0)
		{
			albedo.uniform(shader, "albedo", 0);
			normal.uniform(shader, "normal", 1);
			metallic.uniform(shader, "metallic", 2);
			roughness.uniform(shader, "roughness", 3);
		}
		else if(type[i] == 1)
		{
			sand_albedo.uniform(shader, "albedo", 0);
			sand_normal.uniform(shader, "normal", 1);
			sand_metallic.uniform(shader, "metallic", 2);
			sand_roughness.uniform(shader, "roughness", 3);
		}

		else if (type[i] == 2)
		{
			bamboo_albedo.uniform(shader, "albedo", 0);
			bamboo_normal.uniform(shader, "normal", 1);
			bamboo_metallic.uniform(shader, "metallic", 2);
			bamboo_roughness.uniform(shader, "roughness", 3);
		}

		shader.uniform("model", spheres->model[i]);
		glBindVertexArray(spheres->vao[i]);
		glDrawElements(GL_TRIANGLE_STRIP, spheres->index_count[i], GL_UNSIGNED_INT, 0);
	}
}