#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

camera::camera(float left, float right,
    float bottom, float top)
    : projection(glm::ortho(left, right, bottom, top))
{
    yaw = -80.0f;
    pitch = 0.0f;
    last_x = 0.0f;
    last_y = 0.0f;
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);

    pressed_up = false;
    pressed_down = false;
    pressed_left = false;
    pressed_right = false;
    pressed_shift = false;

    first = true;
}

camera::camera(float fovy, float width,
    float height, float near, float far)
    : projection(glm::perspective(fovy, width / height, near, far))
{
    yaw = -80.0f;
    pitch = 0.0f;
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);

    pressed_up = false;
    pressed_down = false;
    pressed_left = false;
    pressed_right = false;
    pressed_shift = false;

    first = true;
}

void camera::on_mouse_moved(float x, float y)
{
    if(first)
    {
        last_x = x;
        last_y = y;
        first = false;
    }

    auto xoffset = x - last_x;
    auto yoffset = last_y - y;
    last_x = x;
    last_y = y;

    auto sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward = glm::normalize(front);
}

void camera::up_pressed()
{
    pressed_up = true;
}
void camera::up_released()
{
    pressed_up = false;
}
void camera::down_pressed()
{
    pressed_down = true;
}
void camera::down_released()
{
    pressed_down = false;
}
void camera::left_pressed()
{
    pressed_left = true;
}
void camera::left_released()
{
    pressed_left = false;
}
void camera::right_pressed()
{
    pressed_right = true;
}
void camera::right_released()
{
    pressed_right = false;
}

void camera::fast_pressed()
{
    pressed_shift = true;
}

void camera::fast_released()
{
    pressed_shift = false;
}

void camera::update(const std::chrono::milliseconds delta_time)
{
	using namespace std::chrono;
	using float_seconds = duration<float>;
	auto velocity = 10.0f;
	//TURBO SPEED
	if (pressed_shift)
	{
		velocity *= 10;
	}
	velocity *= duration_cast<float_seconds>(delta_time).count();

    if (pressed_up)
        position += forward * velocity;
	if (pressed_left)
        position -= glm::normalize(glm::cross(forward, up)) * velocity;
	if (pressed_down)
        position -= forward * velocity;
	if (pressed_right)
        position += glm::normalize(glm::cross(forward, up)) * velocity;

    view = glm::lookAt(position, position + forward, up);
}

glm::mat4 camera::model_view_projection(const glm::mat4& model) const
{
    return projection * view * model;
}

glm::mat4 camera::get_view() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 camera::get_projection() const
{
	return projection;
}

void camera::bind(const shader& shader)
{
    shader.uniform("view_position", position);
    shader.uniform("view", view);
    shader.uniform("projection", projection);
	shader.uniform("view_projection", projection * view);
}

void camera::move_on_terrain(const scene::terrain& terrain)
{
    position = terrain.calculate_camera_position(position, 4.0f);
}

glm::vec3 camera::get_pos() const
{
	return position;
}

