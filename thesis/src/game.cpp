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

	scene.attach_child(&pl);
	scene.attach_child(&s_box);
	scene.attach_child(&s_quad);

	quad.set_texture("resources/images/brickwall.jpg");
	normal_quad.set_texture("resources/images/brickwall.jpg");
	light_pos = glm::vec3(50, 5, -15);
    phong_pos = glm::vec3(0, 10, 5);

    terrain.update(0.0f);
    for(auto& checkpoint : current_race)
    {
		glm::vec3 v;
		v.x = 20 + rand() % 246;
		v.y = 20;
		v.z = 20 + rand() % 246;

		checkpoint =
            sphere(terrain.calculate_camera_position(v, 2.5f), 2.5f);
    }
    icos.reserve(10 * sizeof(scene::icosahedron));
}

void game::run()
{	
	double timestep = 1.0 / 60.0;
	double last_time = glfwGetTime();
	double delta_time = 0.0f;

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

	phong_shader.use();
	game_camera.bind(phong_shader);
	phong_shader.uniform("light_color", glm::vec3(1.f, 1.f, 1.f));
	phong_shader.uniform("light_pos", phong_pos);
	temp.render(phong_shader);

	basic_shader.use();
	game_camera.bind(basic_shader);
	light.bind(basic_shader);
	scene.render(basic_shader);
	quad.render(basic_shader);

	backface.render(basic_shader);

	terrain_shader.use();
	game_camera.bind(terrain_shader);
	terrain.render(terrain_shader);

	//Normal mapping
	normal_shader.use();
	game_camera.bind(normal_shader);
	normal_shader.uniform("light_pos", light_pos);
	normal_quad.render(normal_shader);

	skybox_shader.use();
	game_camera.bind(skybox_shader);
	skybox_shader.uniform("view",
        glm::mat4(glm::mat3(game_camera.get_view())));

	sky.render(skybox_shader);

	environment_shader.use();
	game_camera.bind(environment_shader);
	environment.render(environment_shader);

	basic_shader.use();
	game_camera.bind(basic_shader);
	for (auto& ics : icos)
	{
		ics.render(basic_shader);
	}

	billboard_shader.use();
	game_camera.bind(billboard_shader);
	particles.render(billboard_shader);

	//Animated model
	anim.use();
	game_camera.bind(anim);
	temp_model.draw(anim);

    if(current_race.lap() < 1)
    {
        p.use();
        game_camera.bind(p);
        emitter.render(p);
    }

	//Text
    text_shader.use();
    glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
    text_shader.uniform("projection", projection);
    text_shader.uniform("text_color", glm::vec3(0.8f, 0.8f, 0.8f));
    temp_text.render_text(ui_text.c_str(), 10, 10, 1);

	game_window.swap_buffers();
}

void game::update(float delta_time)
{
	glm::vec3 cam_pos = game_camera.get_pos();

	for (auto& ico : icos)
	{
		ico.update(delta_time);
	}
    terrain.update(delta_time);
    game_camera.move_on_terrain(terrain);
    game_camera.update(delta_time);
	current_race.update(game_camera.get_pos());

    temp_model.update(delta_time);

	particles.update(delta_time);

	if (race_index == current_race.get_checkpoint() && race_index < 10)
	{
		if(!icos.empty())
        {
            icos.back().set_color(glm::vec3(0.1f, 1.0f, 0.1f));
            icos.back().clear();
        }
        auto& p = current_race[race_index].position;
		icos.emplace_back(p.x, p.y, p.z);
        icos.back().attach_child(&emitter);
		ui_text = std::to_string(race_index) + " / 10";
		++race_index;
	}

    if(current_race.lap() > 0)
    {
        ui_text = "victory!";
        color_timer += delta_time;
        if (color_timer > 0.500)
        {
            color_timer = 0.0;
            for (auto& obj : icos)
            {
                 obj.set_color(glm::vec3(
                    (rand() % 255) / 255.f,
                    (rand() % 255) / 255.f,
                    (rand() % 255) / 255.f));
            }
		}
	}

    seconds += delta_time;
    light_pos.x += glm::sin(seconds * 2.0f);
	light_pos.y += glm::sin(seconds * 0.7f);
	phong_pos.x += sin(seconds * 2.0f) / 10.0f;
}
