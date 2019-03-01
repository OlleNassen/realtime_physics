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
#include "animation.hpp"
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

	camera game_camera;
	directional_light light;

	anim::model temp_model;

	scene::node scene;
	scene::skybox sky;
	scene::terrain terrain{0, 0, 0};

	text temp_text;

	world physics_world;

	glm::vec3 light_pos;
	glm::vec3 phong_pos;
	float seconds = 0.0f;
	float color_timer = 0.0f;
	std::string ui_text;

	cam3p ThirdPersonCamera = {};

	void render();
	void update(float delta_time);
};

#endif
