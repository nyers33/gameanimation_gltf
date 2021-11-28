#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <cmath>

inline glm::vec3 transformVector(const glm::mat4& m, const glm::vec3& v) {
	glm::vec4 out = m * glm::vec4(v, 0.0f);
	return glm::vec3(out.x, out.y, out.z);
}

inline glm::vec3 transformPoint(const glm::mat4& m, const glm::vec3& v) {
	glm::vec4 out = m * glm::vec4(v, 1.0f);
	return glm::vec3(out.x, out.y, out.z);
}

inline glm::quat fromTo(const glm::vec3& from, const glm::vec3& to) {
	glm::vec3 in_from = glm::normalize(from);
	glm::vec3 in_to = glm::normalize(to);

	glm::quat q = glm::rotation(in_from, in_to);

	return glm::normalize(q);
}

inline glm::mat4 perspective(float fov, float aspect, float znear, float zfar) {
	float ymax = znear * tanf(fov * 3.14159265359f / 360.0f);
	float xmax = ymax * aspect;

	return glm::frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}