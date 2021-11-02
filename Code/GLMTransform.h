#ifndef _H_GLMTRANSFORM_
#define _H_GLMTRANSFORM_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLMTransform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	GLMTransform() :
		position(glm::vec3(0, 0, 0)),
		rotation(glm::quat(0, 0, 0, 1)),
		scale(glm::vec3(1, 1, 1)) {}
	GLMTransform(const glm::vec3& p, const glm::quat& r, const glm::vec3& s) :
		position(p), rotation(r), scale(s) {}
}; // End of transform struct

GLMTransform combine(const GLMTransform& a, const GLMTransform& b);
GLMTransform inverse(const GLMTransform& t);
GLMTransform mix(const GLMTransform& a, const GLMTransform& b, float t);
bool operator==(const GLMTransform& a, const GLMTransform& b);
bool operator!=(const GLMTransform& a, const GLMTransform& b);
glm::mat4 transformToMat4(const GLMTransform& t);
GLMTransform mat4ToTransform(const glm::mat4& m);
glm::vec3 transformPoint(const GLMTransform& a, const glm::vec3& b);
glm::vec3 transformVector(const GLMTransform& a, const glm::vec3& b);

#endif