#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <chrono>

#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "directional_light.hpp"
#include "node.hpp"
#include "skybox.hpp"
#include "terrain.hpp"
#include "text.hpp"

#include "cam3p.hpp"
#include "physics.hpp"

struct ball
{
	glm::vec3 position;
	float radius;
};

std::ostream& operator<<(std::ostream& os, const glm::mat4& value);
using namespace std::literals::chrono_literals;

static constexpr auto width = 1280u;
static constexpr auto height = 720u;

typedef float seconds;

class game
{
public:
	game();

	void run();

private:
	window game_window{width, height, "VOILA"};

	shader skybox_shader{"resources/shaders/skybox.vs", "resources/shaders/skybox.fs"};
	shader anim{"resources/shaders/anim.vs", "resources/shaders/anim.fs"};
	shader terrain_shader{"resources/shaders/terrain.vs", "resources/shaders/terrain.fs"};
	shader text_shader{"resources/shaders/text.vs", "resources/shaders/text.fs"};
	shader triangle_shader{"resources/shaders/lol.vs", "resources/shaders/lol.fs"};

	camera game_camera;
	directional_light light;

	scene::node scene;
	scene::skybox sky;
	scene::terrain terrain{0, 0, 0};

	text temp_text;

	world physics_world;

	float seconds = 0.0f;
	float color_timer = 0.0f;
	std::string ui_text;

	cam3p ThirdPersonCamera = {};

	int terrain_type = 0;
	int ball_material[2];

	bool is_verlet = true;

	void render();
	void update(float delta_time);
};

#endif
