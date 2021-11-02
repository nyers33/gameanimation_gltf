#include "GLMTransform.h"
#include <cmath>
#include <iostream>

using namespace glm;

GLMTransform combine(const GLMTransform& a, const GLMTransform& b) {
	GLMTransform out;

	out.scale = a.scale * b.scale;
	out.rotation = a.rotation * b.rotation;

	out.position = a.rotation * (a.scale * b.position);
	out.position = a.position + out.position;

	return out;
}

GLMTransform inverse(const GLMTransform& t) {
	GLMTransform inv;

	inv.rotation = inverse(t.rotation);

	const float VEC3_EPSILON = 0.000001f;

	inv.scale.x = fabs(t.scale.x) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.x;
	inv.scale.y = fabs(t.scale.y) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.y;
	inv.scale.z = fabs(t.scale.z) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.z;

	vec3 invTranslation = t.position * -1.0f;
	inv.position = inv.rotation * (inv.scale * invTranslation);

	return inv;
}

GLMTransform mix(const GLMTransform& a, const GLMTransform& b, float t) {
	quat bRot = b.rotation;
	if (dot(a.rotation, bRot) < 0.0f) {
		bRot = -bRot;
	}
	return GLMTransform(
		glm::mix(a.position, b.position, t),
		glm::normalize(glm::mix(a.rotation, bRot, t)),
		glm::mix(a.scale, b.scale, t));
}

bool operator==(const GLMTransform& a, const GLMTransform& b) {
	return a.position == b.position &&
		a.rotation == b.rotation &&
		a.scale == b.scale;
}

bool operator!=(const GLMTransform& a, const GLMTransform& b) {
	return !(a == b);
}

mat4 transformToMat4(const GLMTransform& t) {
	// First, extract the rotation basis of the transform
	vec3 x = t.rotation * vec3(1, 0, 0);
	vec3 y = t.rotation * vec3(0, 1, 0);
	vec3 z = t.rotation * vec3(0, 0, 1);

	// Next, scale the basis vectors
	x = x * t.scale.x;
	y = y * t.scale.y;
	z = z * t.scale.z;

	// Extract the position of the transform
	vec3 p = t.position;

	// Create matrix
	return mat4(
		x.x, x.y, x.z, 0, // X basis (& Scale)
		y.x, y.y, y.z, 0, // Y basis (& scale)
		z.x, z.y, z.z, 0, // Z basis (& scale)
		p.x, p.y, p.z, 1  // Position
	);
}

GLMTransform mat4ToTransform(const mat4& m) {
	GLMTransform out;

	out.position = vec3(m[3][0], m[3][1], m[3][2]);
	out.rotation = glm::quat_cast(m);

	mat4 rotScaleMat(
		m[0][0], m[0][1], m[0][2], 0,
		m[1][0], m[1][1], m[1][2], 0,
		m[2][0], m[2][1], m[2][2], 0,
		0, 0, 0, 1
	);
	mat4 invRotMat = glm::mat4_cast(inverse(out.rotation));
	mat4 scaleSkewMat = rotScaleMat * invRotMat;

	out.scale = vec3(
		scaleSkewMat[0][0],
		scaleSkewMat[1][1],
		scaleSkewMat[2][2]
	);

	return out;
}

vec3 transformPoint(const GLMTransform& a, const vec3& b) {
	vec3 out;

	out = a.rotation * (a.scale * b);
	out = a.position + out;

	return out;
}

vec3 transformVector(const GLMTransform& a, const vec3& b) {
	vec3 out;

	out = a.rotation * (a.scale * b);

	return out;
}