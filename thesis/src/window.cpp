#include "window.hpp"
#include <iostream>
#include <map>

window::window(
    unsigned int width,
    unsigned int height,
    const std::string& title)
{
    if(!glfwInit())
    {
        std::cout << "Failed to init glfw";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window = glfwCreateWindow(width, height,
        title.c_str(), nullptr, nullptr);

    if (!glfw_window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
    }

    glfwMakeContextCurrent(glfw_window);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, width, height);

    glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error glew init failed" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
}

window::~window()
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

void window::on_escape()
{
    glfwSetWindowShouldClose(glfw_window, true);
}

void window::callback(
        GLFWcursorposfun cursor_position_callback,
        GLFWkeyfun key_callback)
{
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    glfwSetKeyCallback(glfw_window, key_callback);
}

bool window::is_open() const
{
    return !glfwWindowShouldClose(glfw_window);
}

void window::swap_buffers()
{
    glfwSwapBuffers(glfw_window);
}

void window::poll_events()
{
    glfwPollEvents();
}

std::function<void(float, float)> mouse_callback;
std::map<std::string, std::function<void()>> keymap_pressed;
std::map<std::string, std::function<void()>> keymap_released;
std::map<key, std::string> keybinds;

void bind_key(const std::string& name, key keybind)
{
    keybinds.insert(std::pair<key, std::string>(keybind, name));
}

void key_callback(GLFWwindow* window, int key_id, int scancode,
    int action, int mods)
{
    auto name = keybinds[static_cast<key>(key_id)];

    if(action == GLFW_PRESS)
    {
        auto callback = keymap_pressed[name];
        if(callback)
        {
            callback();
        }
    }
    if(action == GLFW_RELEASE)
    {
        auto callback = keymap_released[name];
        if(callback)
        {
            callback();
        }
    }

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(mouse_callback)
    {
        mouse_callback(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}



void assign_window(window& callback_window)
{
    callback_window.callback(
        cursor_position_callback,
        key_callback);
}

void assign_mouse_callback(std::function<void(float, float)> callback)
{
    mouse_callback = callback;
}

void assign_key_callback(const std::string& name,
        std::function<void()> pressed,
        std::function<void()> released)
{
    keymap_pressed.insert(std::pair<std::string,
        std::function<void()>>(name, pressed));

    keymap_released.insert(std::pair<std::string,
        std::function<void()>>(name, released));
}
