#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <chrono>

#include "window.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "directional_light.hpp"
#include "node.hpp"
#include "box.hpp"
#include "skybox.hpp"
#include "terrain.hpp"
#include "quad.hpp"
#include "point_light.hpp"
#include "temp_box.hpp"
#include "normal_quad.hpp"
#include "particle_emitter.hpp"
#include "animation.hpp"
#include "quad_tess.hpp"
#include "icosahedron.hpp"
#include "race.hpp"
#include "text.hpp"
#include "particles.hpp"

std::ostream& operator<<(std::ostream& os, const glm::mat4& value);
using namespace std::literals::chrono_literals;

static constexpr auto timestep = 16ms;
static constexpr auto width = 1280u;
static constexpr auto height = 720u;

class game
{
public:
	game();

	void run();

private:
	window game_window{width, height, "VOILA"};
	shader basic_shader{"resources/shaders/basic.vs", "resources/shaders/basic.fs"};
	shader skybox_shader{"resources/shaders/skybox.vs", "resources/shaders/skybox.fs"};
	shader phong_shader{"resources/shaders/phong.vs", "resources/shaders/phong.fs"};
	shader normal_shader{"resources/shaders/normal.vs", "resources/shaders/normal.fs"};
	shader anim{"resources/shaders/anim.vs", "resources/shaders/anim.fs"};
	shader billboard_shader{"resources/shaders/billboard.vs", "resources/shaders/billboard.fs"};
	shader terrain_shader{"resources/shaders/terrain.vs", "resources/shaders/terrain.fs"};
	shader environment_shader{"resources/shaders/environment_mapping.vs",
        "resources/shaders/environment_mapping.fs"};
	shader text_shader{"resources/shaders/text.vs", "resources/shaders/text.fs"};
	shader tess_shader{"resources/shaders/tess.vs", "resources/shaders/tess.cs",
        "resources/shaders/tess.es", "resources/shaders/tess.fs"};
	shader backface_shader{ "resources/shaders/backface.vs",
        "resources/shaders/backface.gs", "shaders/backface.fs" };
	shader p{ "resources/shaders/particle.vs", "resources/shaders/particle.fs" };


	camera game_camera;
	directional_light light;

	anim::model temp_model;

	scene::point_light pl;
	scene::box s_box{20, 38, 40};
	scene::quad s_quad{20, 38, 20};

	scene::node scene;
	scene::skybox sky;
	scene::temp_box temp{0, 10, 0}; // PHONG cube
	scene::temp_box environment{10, 2, 0};
	scene::quad quad{0, 5, -20}; // phong
	scene::normal_quad normal_quad{50, 5, -20};
	scene::particle_emitter particles;
	scene::terrain terrain{10, 10, 10};
	scene::quad_tess quad_tess;
	scene::quad backface{0, 0, -10};
	scene::particles::emitter emitter;

	text temp_text;

	int race_index{0};
    race current_race;
	std::vector<scene::icosahedron> icos;

	glm::vec3 light_pos;
	glm::vec3 phong_pos;
	std::chrono::duration<float> seconds{0s};
	std::chrono::milliseconds color_timer{0ms};
	std::string ui_text;

	void render();
	void update(std::chrono::milliseconds delta_time);
};

#endif
