#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

shader::shader(const std::string& vertex_path, const std::string& fragment_path)
{
    auto vertex_code = load(vertex_path);
    auto fragment_code = load(fragment_path);

    auto vertex_shader = create(GL_VERTEX_SHADER, vertex_code.c_str());
    auto fragment_shader = create(GL_FRAGMENT_SHADER, fragment_code.c_str());

    auto success = 0;
    char infoLog[512];

    id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    // print linking errors if any
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Error linking failed: "
            << infoLog  << " in " << vertex_path
			<< " or " << fragment_path << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

shader::shader(const std::string& vertex_path,
	const std::string& geo_path, const std::string& fragment_path)
{
	auto vertex_code = load(vertex_path);
	auto geo_code(load(geo_path));
	auto fragment_code = load(fragment_path);

	auto vertex_shader = create(GL_VERTEX_SHADER, vertex_code.c_str());
	auto geo_shader = create(GL_GEOMETRY_SHADER, geo_code.c_str());
	auto fragment_shader = create(GL_FRAGMENT_SHADER, fragment_code.c_str());

	auto success = 0;
	char infoLog[512];

	id = glCreateProgram();
	glAttachShader(id, vertex_shader);
	glAttachShader(id, geo_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);
	// print linking errors if any
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Error linking failed: " << infoLog << " in "
            << vertex_path << " or " << fragment_path << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(geo_shader);
	glDeleteShader(fragment_shader);
}

shader::shader(const std::string & vertex_path,
    const std::string & tess_control,
    const std::string & tess_eval,
    const std::string & fragment_path)
{
	std::string vertex_code(load(vertex_path));
	std::string control_code(load(tess_control));
	std::string eval_code(load(tess_eval));
	std::string fragment_code(load(fragment_path));

	auto vertex_shader = create(GL_VERTEX_SHADER, vertex_code.c_str());
	auto control_shader = create(GL_TESS_CONTROL_SHADER, control_code.c_str());
	auto eval_shader = create(GL_TESS_EVALUATION_SHADER, eval_code.c_str());
	auto fragment_shader = create(GL_FRAGMENT_SHADER, fragment_code.c_str());

	auto success = 0;
	char infoLog[512];

	id = glCreateProgram();
	glAttachShader(id, vertex_shader);
	glAttachShader(id, control_shader);
	glAttachShader(id, eval_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);
	// print linking errors if any
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Error linking failed: " << infoLog
            << " in " << vertex_path
			<< " or " << fragment_path << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(control_shader);
	glDeleteShader(eval_shader);
	glDeleteShader(fragment_shader);
}

shader::~shader()
{
    glDeleteProgram(id);
}

void shader::use() const
{
    glUseProgram(id);
}

void shader::uniform(const std::string& name, int value) const
{
    glUniform1iv(glGetUniformLocation(id, name.c_str()),
        1, &value);
}

void shader::uniform(const std::string& name, float value) const
{
    glUniform1fv(glGetUniformLocation(id, name.c_str()),
        1, &value);
}

void shader::uniform(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name,
    const glm::vec<4, int, glm::highp>& value) const
{
    glUniform4iv(glGetUniformLocation(id, name.c_str()),
        1, glm::value_ptr(value));
}

void shader::uniform(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
        1, GL_FALSE, glm::value_ptr(value));
}

void shader::uniform(const std::string& name,
    const std::vector<int>& value) const
{
    if(value.size() > 0)
    {
        glUniform1iv(glGetUniformLocation(id, name.c_str()),
            value.size(), &value.front());
    }
}

void shader::uniform(const std::string& name,
    const std::vector<float>& value) const
{
    if(value.size() > 0)
    {
        glUniform1fv(glGetUniformLocation(id, name.c_str()),
            value.size(), &value.front());
    }
}

void shader::uniform(const std::string& name,
    const std::vector<glm::vec2>& value) const
{
    if(value.size() > 0)
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()),
            value.size(), glm::value_ptr(value.front()));
    }
}

void shader::uniform(const std::string& name,
    const std::vector<glm::vec3>& value) const
{
    if(value.size() > 0)
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()),
            value.size(), glm::value_ptr(value.front()));
    }
}

void shader::uniform(const std::string& name,
    const std::vector<glm::vec4>& value) const
{
    if(value.size() > 0)
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()),
            value.size(), glm::value_ptr(value.front()));
    }
}

void shader::uniform(const std::string& name,
    const std::vector<glm::mat4>& value) const
{
    if(value.size() > 0)
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
            value.size(), GL_FALSE, glm::value_ptr(value.front()));
    }
}

void shader::uniform(const std::string& name,
    const std::array<glm::mat4, 50>& value) const
{
    if(value.size() > 0)
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
            value.size(), GL_FALSE, glm::value_ptr(value.front()));
    }
}

std::string shader::load(const std::string& path) const
{
    std::string code;
    std::ifstream shader_file;
    std::stringstream shader_stream;

    shader_file.open(path);
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    code = shader_stream.str();

    return code;
}

unsigned int shader::create(unsigned int shader_type,
    const char* shader_code) const
{
    auto shader_id = 0;
    auto success = 0;
    char infoLog[512];

    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
        std::cout << "Error compile failed: " << infoLog << std::endl;
    };

    return shader_id;
}
