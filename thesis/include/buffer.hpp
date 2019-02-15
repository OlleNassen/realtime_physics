#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <array>
#include <vector>
#include <tuple>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture.hpp"

class buffer
{
public:
    buffer(unsigned int buffer_target)
        : buffer()
    {
        this->buffer_target = buffer_target;
    }
    buffer(){glGenBuffers(1, &id);}
    ~buffer(){glDeleteBuffers(1, &id);}
    void bind() const{glBindBuffer(buffer_target, id);}

    void data(int size, const void* data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target, size, data, usage);
    }

    void sub_data(int offset, int size,
        const void* data) const
    {
        glBufferSubData(buffer_target, offset, size, data);
    }

    template<typename T, unsigned int N>
    void data(const std::array<T, N>& data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target,
            sizeof(T) * data.size(), data.data(), usage);
    }

    template<typename T>
    void data(const std::vector<T>& data, unsigned int usage) const
    {
        bind();
        glBufferData(buffer_target,
            sizeof(T) * data.size(), data.data(), usage);
    }

private:
    unsigned int buffer_target = GL_ARRAY_BUFFER;
    unsigned int id = 0u;

};

class vertex_array
{
public:
	vertex_array(){glGenVertexArrays(1, &id);}

	~vertex_array(){glDeleteVertexArrays(1, &id);}

	void bind() const{glBindVertexArray(id);}

	void attribute_pointer(unsigned int index, int size, unsigned int type,
		unsigned char normalized, int stride, const void* pointer) const
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

	void attribute_pointer(unsigned int index, int size, unsigned int type,
		int stride, const void* pointer) const
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(index, size, type, stride, pointer);
    }

	void attribute_divisor(unsigned int index, unsigned int divisor) const
    {
        bind();
        glVertexAttribDivisor(index, divisor);
    }

private:
	unsigned int id;
};

#endif // BUFFER_HPP
