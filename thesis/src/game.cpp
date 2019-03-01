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

	physics_world.player_position.old_position.x = 35.0f;
	physics_world.player_position.old_position.y = 70.0f;
	physics_world.player_position.old_position.z = 35.0f;

	physics_world.player_position.position.x = 35.0f;
	physics_world.player_position.position.y = 70.0f;
	physics_world.player_position.position.z = 35.0f;
	
	physics_world.player_collider.radius = 2.0f;

	for (auto& vertex : terrain.vertices)
	{
		plane p;
		p.position = vertex.position;
		p.normal = vertex.normal;
		physics_world.planes.emplace_back(p);
	}
}

void game::run()
{	
	double timestep = 1.0 / 60.0;
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


	terrain_shader.use();
	BindCamera(&ThirdPersonCamera, &terrain_shader);
	//game_camera.bind(terrain_shader);
	terrain.render(terrain_shader);

	skybox_shader.use();
	BindCamera(&ThirdPersonCamera, &skybox_shader);
	//game_camera.bind(skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(glm::mat3(game_camera.get_view())));

	sky.render(skybox_shader);

	//Animated model
	anim.use();
	BindCamera(&ThirdPersonCamera, &anim);
	//game_camera.bind(anim);
	temp_model.draw(anim);

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	glm::vec3 cam_pos = game_camera.get_pos();

    terrain.update(delta_time);
    game_camera.move_on_terrain(terrain);
    game_camera.update(delta_time);

    temp_model.update(delta_time);

    seconds += delta_time;
    light_pos.x += glm::sin(seconds * 2.0f);
	light_pos.y += glm::sin(seconds * 0.7f);
	phong_pos.x += sin(seconds * 2.0f) / 10.0f;

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
}
