#ifndef _H_TRANSFORM_
#define _H_TRANSFORM_

#include "anim_glm.h"

struct Transform {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	Transform() :
		position(glm::vec3(0, 0, 0)),
		rotation(glm::quat(1, 0, 0, 0)),
		scale(glm::vec3(1, 1, 1)) {}
	Transform(const glm::vec3& p, const glm::quat& r, const glm::vec3& s) :
		position(p), rotation(r), scale(s) {}
}; // End of transform struct

Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);
Transform mix(const Transform& a, const Transform& b, float t);
bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);
glm::mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const glm::mat4& m);
glm::vec3 transformPoint(const Transform& a, const glm::vec3& b);
glm::vec3 transformVector(const Transform& a, const glm::vec3& b);

#endif