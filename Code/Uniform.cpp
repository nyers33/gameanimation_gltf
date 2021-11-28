#include "Uniform.h"
#include <GL/glew.h>
#include "anim_glm.h"

template class Uniform<int>;
template class Uniform<glm::ivec4>;
template class Uniform<glm::ivec2>;
template class Uniform<float>;
template class Uniform<glm::vec2>;
template class Uniform<glm::vec3>;
template class Uniform<glm::vec4>;
template class Uniform<glm::quat>;
template class Uniform<glm::mat4>;

#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> \
void Uniform<tType>::Set(unsigned int slot, tType* data, unsigned int length) { \
    gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
}

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, glm::ivec4, int)
UNIFORM_IMPL(glUniform2iv, glm::ivec2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, glm::vec2, float)
UNIFORM_IMPL(glUniform3fv, glm::vec3, float)
UNIFORM_IMPL(glUniform4fv, glm::vec4, float)
UNIFORM_IMPL(glUniform4fv, glm::quat, float)

template<>
void Uniform<glm::mat4>::Set(unsigned int slot, glm::mat4* inputArray, unsigned int arrayLength) {
	glUniformMatrix4fv(slot, (GLsizei)arrayLength, false, (float*)&inputArray[0]);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, const T& value) {
	Set(slot, (T*)&value, 1);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, std::vector<T>& value) {
	Set(slot, &value[0], (unsigned int)value.size());
}
