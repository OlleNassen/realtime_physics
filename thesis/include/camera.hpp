#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <chrono>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "terrain.hpp"

class camera
{
public:
    camera(float left, float right,
        float bottom, float top);

    camera(float fovy, float width,
        float height, float near, float far);

    void on_mouse_moved(float x, float y);
    void up_pressed();
    void up_released();
    void down_pressed();
    void down_released();
    void left_pressed();
    void left_released();
    void right_pressed();
    void right_released();
    void fast_pressed();
    void fast_released();

    void update(const float delta_time);
    glm::mat4 model_view_projection(const glm::mat4& model) const;
	glm::mat4 get_view()const;
	glm::mat4 get_projection()const;
    void bind(const shader& shader);

	void move_on_terrain(const scene::terrain& terrain);

	glm::vec3 get_pos()const;

private:
    bool pressed_up;
    bool pressed_down;
    bool pressed_left;
    bool pressed_right;
    bool pressed_shift;

    bool first;
    float yaw;
	float pitch;
	float last_x;
	float last_y;

    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
};

#endif // CAMERA_HPP
