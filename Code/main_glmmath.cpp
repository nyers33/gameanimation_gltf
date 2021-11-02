#pragma warning(disable : 28251)
#pragma warning(disable : 28159)

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <gtest/gtest.h>

#define GLM_FORCE_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "quat.h"
#include "vec4.h"
#include "mat4.h"

#include "Transform.h"
#include "GLMTransform.h"

//EXPECT_FLOAT_EQ(expected, actual)
//EXPECT_DOUBLE_EQ(expected, actual)
//EXPECT_NEAR(expected, actual, absolute_range)

void expect_vec3_eq(const vec3& expected, const glm::vec3& actual, float absolute_range = 0.0f)
{
	if (absolute_range == 0.0f)
	{
		EXPECT_FLOAT_EQ(expected.x, actual.x);
		EXPECT_FLOAT_EQ(expected.y, actual.y);
		EXPECT_FLOAT_EQ(expected.z, actual.z);
	}
	else
	{
		EXPECT_NEAR(expected.x, actual.x, absolute_range);
		EXPECT_NEAR(expected.y, actual.y, absolute_range);
		EXPECT_NEAR(expected.z, actual.z, absolute_range);
	}
}

void expect_vec4_eq(const vec4& expected, const glm::vec4& actual, float absolute_range = 0.0f)
{
	if (absolute_range == 0.0f)
	{
		EXPECT_FLOAT_EQ(expected.x, actual.x);
		EXPECT_FLOAT_EQ(expected.y, actual.y);
		EXPECT_FLOAT_EQ(expected.z, actual.z);
		EXPECT_FLOAT_EQ(expected.w, actual.w);
	}
	else
	{
		EXPECT_NEAR(expected.x, actual.x, absolute_range);
		EXPECT_NEAR(expected.y, actual.y, absolute_range);
		EXPECT_NEAR(expected.z, actual.z, absolute_range);
		EXPECT_NEAR(expected.w, actual.w, absolute_range);
	}
}

void expect_quat_eq(const quat& expected, const glm::quat& actual, float absolute_range = 0.0f)
{
	if (absolute_range == 0.0f)
	{
		EXPECT_FLOAT_EQ(expected.x, actual.x);
		EXPECT_FLOAT_EQ(expected.y, actual.y);
		EXPECT_FLOAT_EQ(expected.z, actual.z);
		EXPECT_FLOAT_EQ(expected.w, actual.w);
	}
	else
	{
		EXPECT_NEAR(expected.x, actual.x, absolute_range);
		EXPECT_NEAR(expected.y, actual.y, absolute_range);
		EXPECT_NEAR(expected.z, actual.z, absolute_range);
		EXPECT_NEAR(expected.w, actual.w, absolute_range);
	}
}

void expect_mat4_eq(const mat4& expected, const glm::mat4& actual)
{
	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			EXPECT_FLOAT_EQ(expected.v[i + j * 4], actual[j][i]);
		}
	}
}

TEST(Vec3, AngleProjRefl)
{
	vec3 a0 = vec3(0.7f, 0.2f, -1.7f);
	vec3 b0 = vec3(0.17f, -1.3f, 0.33f);

	float angle0 = angle(a0, b0);
	vec3 proj0 = project(a0, b0);
	vec3 refl0 = reflect(a0, b0);

	glm::vec3 a1 = glm::vec3(0.7f, 0.2f, -1.7f);
	glm::vec3 b1 = glm::vec3(0.17f, -1.3f, 0.33f);

	// Returns the absolute angle between two vectors Parameters need to be normalized.
	float angle1 = glm::angle(glm::normalize(a1), glm::normalize(b1));

	// Projects x on Normal.
	glm::vec3 proj1 = glm::proj(a1, b1);
	
	// For the incident vector I and surface orientation N, returns the reflection direction : result = I - 2.0 * dot(N, I) * N.
	glm::vec3 refl1 = glm::reflect(a1, glm::normalize(b1));

	EXPECT_FLOAT_EQ(angle0, angle1);
	expect_vec3_eq(proj0, proj1);
	expect_vec3_eq(refl0, refl1);
}

TEST(Quat, AngleAxis)
{
	const float kEpsilon = 0.0001f;

	quat q0 = angleAxis(0.33f, vec3(0.5f, 0.5f, 0.5f));
	vec3 axis0 = getAxis(q0);
	float angle0 = getAngle(q0);

	glm::quat q1 = glm::angleAxis(0.33f, glm::normalize(glm::vec3(0.5f, 0.5f, 0.5f)));
	glm::vec3 axis1 = glm::normalize(glm::axis(q1));
	float angle1 = glm::angle(q1);

	expect_quat_eq(q0, q1);
	expect_vec3_eq(axis0, axis1);
	EXPECT_NEAR(angle0, angle1, kEpsilon);
}

TEST(Quat, QuaternionFunctions)
{
	quat q0 = angleAxis(0.66f, vec3(0.5f, 0.75f, 0.5f));
	quat q00 = conjugate(q0);
	quat q000 = inverse(q00);

	glm::quat q1 = glm::angleAxis(0.66f, glm::normalize(glm::vec3(0.5f, 0.75f, 0.5f)));
	glm::quat q11 = glm::conjugate(q1);
	glm::quat q111 = inverse(q11);

	expect_quat_eq(q0, q1);
	expect_quat_eq(q00, q11);
	expect_quat_eq(q000, q111);
}

TEST(Quat, MixSlerp)
{
	const float kEpsilon = 0.0025f;

	quat q0 = angleAxis(0.66f, vec3(0.5f, 0.75f, 0.5f));
	quat q00 = angleAxis(0.75f, vec3(0.35f, 0.25f, 0.15f));

	quat q000_0 = mix(q0, q00, 0.0f);
	quat q000_1 = mix(q0, q00, 0.5f);
	quat q000_2 = mix(q0, q00, 1.0f);

	quat q0000_0 = slerp(q0, q00, 0.0f);
	quat q0000_1 = slerp(q0, q00, 0.5f);
	quat q0000_2 = slerp(q0, q00, 1.0f);

	glm::quat q1 = glm::angleAxis(0.66f, glm::normalize(glm::vec3(0.5f, 0.75f, 0.5f)));
	glm::quat q11 = glm::angleAxis(0.75f, glm::normalize(glm::vec3(0.35f, 0.25f, 0.15f)));

	glm::quat q111_0 = glm::mix(q1, q11, 0.0f);
	glm::quat q111_1 = glm::mix(q1, q11, 0.5f);
	glm::quat q111_2 = glm::mix(q1, q11, 1.0f);

	glm::quat q1111_0 = glm::slerp(q1, q11, 0.0f);
	glm::quat q1111_1 = glm::slerp(q1, q11, 0.5f);
	glm::quat q1111_2 = glm::slerp(q1, q11, 1.0f);

	expect_quat_eq(q0, q1);
	expect_quat_eq(q00, q11);

	expect_quat_eq(q000_0, q111_0, kEpsilon);
	expect_quat_eq(q000_1, q111_1, kEpsilon);
	expect_quat_eq(q000_2, q111_2, kEpsilon);

	expect_quat_eq(q0000_0, q1111_0, kEpsilon);
	expect_quat_eq(q0000_1, q1111_1, kEpsilon);
	expect_quat_eq(q0000_2, q1111_2, kEpsilon);
}

TEST(Quat, Quat2Mat)
{
	quat q0 = angleAxis(0.33f, vec3(0.5f, 0.5f, 0.5f));
	mat4 m0 = quatToMat4(q0);
	quat q00 = mat4ToQuat(m0);

	glm::quat q1 = glm::angleAxis(0.33f, glm::normalize(glm::vec3(0.5f, 0.5f, 0.5f)));
	glm::mat4 m1 = glm::mat4_cast(q1);
	glm::quat q11 = glm::quat_cast(m1);

	expect_quat_eq(q0, q1);
	expect_mat4_eq(m0, m1);
	expect_quat_eq(q00, q11);
}

TEST(Mat4, Entity)
{
	mat4 m0 = mat4();
	glm::mat4 m1 = glm::mat4(1.0f);

	expect_mat4_eq(m0, m1);
}

TEST(Mat4, MatrixFunctions)
{
	float arr[] = {
		1,0,0,1,
		0,2,1,2,
		2,1,0,1,
		2,0,1,4
	};

	mat4 m0 = mat4(arr);
	float det0 = determinant(m0);
	mat4 inv0 = inverse(m0);

	glm::mat4 m1 = glm::make_mat4(arr);
	float det1 = glm::determinant(m1);
	glm::mat4 inv1 = glm::inverse(m1);

	EXPECT_FLOAT_EQ(det0, det1);
	expect_mat4_eq(inv0, inv1);
}

TEST(Mat4, CommonTransformationMatrices)
{
	float l = 1;
	float r = 2;
	float b = 3;
	float t = 4;
	float n = 5;
	float f = 6;

	mat4 m0 = frustum(l, r, b, t, n, f);
	glm::mat4 m1 = glm::frustum(l, r, b, t, n, f);

	expect_mat4_eq(m0, m1);

	mat4 m00 = perspective(60.0f, 1.33f, 0.01f, 1000.0f);
	glm::mat4 m11 = glm::perspective(glm::radians(60.0f), 1.33f, 0.01f, 1000.0f);

	expect_mat4_eq(m00, m11);

	mat4 m000 = lookAt(vec3(0, 5, 7), vec3(0, 3, 0), vec3(0, 1, 0));
	glm::mat4 m111 = glm::lookAt(glm::vec3(0, 5, 7), glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));

	expect_mat4_eq(m000, m111);
}

TEST(GLMTransform, CommonFunctions)
{
	const float kEpsilon = 0.0025f;

	vec3 pos0(0, 5, 0);
	vec3 pos00(3, -7, 10);
	quat rot0 = angleAxis(0.33f, vec3(0.5f, 0.5f, 0.5f));
	quat rot00 = angleAxis(-0.66f, vec3(0.15f, -0.75f, 0.5f));
	vec3 scale0(2, 2, 2);
	vec3 scale00(3, 4, 5);

	Transform traf0(pos0, rot0, scale0);
	Transform traf00(pos00, rot00, scale00);

	glm::vec3 pos1(0, 5, 0);
	glm::vec3 pos11(3, -7, 10);
	glm::quat rot1 = glm::angleAxis(0.33f, glm::normalize(glm::vec3(0.5f, 0.5f, 0.5f)));
	glm::quat rot11 = glm::angleAxis(-0.66f, glm::normalize(glm::vec3(0.15f, -0.75f, 0.5f)));
	glm::vec3 scale1(2, 2, 2);
	glm::vec3 scale11(3, 4, 5);

	GLMTransform traf1(pos1, rot1, scale1);
	GLMTransform traf11(pos11, rot11, scale11);

	{
		Transform traf000 = inverse(traf0);
		GLMTransform traf111 = inverse(traf1);

		expect_vec3_eq(traf000.position, traf111.position);
		expect_quat_eq(traf000.rotation, traf111.rotation);
		expect_vec3_eq(traf000.scale, traf111.scale);
	}

	{
		expect_vec3_eq(pos0, pos1);
		expect_vec3_eq(pos00, pos11);

		expect_quat_eq(rot0, rot1);
		expect_quat_eq(rot00, rot11);

		expect_vec3_eq(scale0, scale1);
		expect_vec3_eq(scale00, scale11);

		quat rot000 = rot0 * rot00;
		glm::quat rot111 = rot11 * rot1;

		if(0)
		{
			// https://gist.github.com/SIRHAMY/9767ed75bddf0b87b929
			glm::vec3 q1(rot1.x, rot1.y, rot1.z);
			glm::vec3 q2(rot11.x, rot11.y, rot11.z);

			rot000.w = rot1.w * rot11.w - glm::dot(q1,q2);
			
			glm::vec3 rotVec = rot1.w * q2 + rot11.w * q1 + glm::cross(q1, q2);
			rot000.x = rotVec.x;
			rot000.y = rotVec.y;
			rot000.z = rotVec.z;
		}

		expect_quat_eq(rot000, rot111);
	}

	{
		Transform traf000 = combine(traf0, traf00);
		GLMTransform traf111 = combine(traf1, traf11);

		expect_vec3_eq(traf000.position, traf111.position, kEpsilon);
		expect_quat_eq(traf000.rotation, traf111.rotation, kEpsilon);
		expect_vec3_eq(traf000.scale, traf111.scale, kEpsilon);
	}

	{
		Transform traf000 = mix(traf0, traf00, 0.33f);
		GLMTransform traf111 = mix(traf1, traf11, 0.33f);

		expect_vec3_eq(traf000.position, traf111.position, kEpsilon);
		expect_quat_eq(traf000.rotation, traf111.rotation, kEpsilon);
		expect_vec3_eq(traf000.scale, traf111.scale, kEpsilon);
	}

	{
		mat4 mat0 = transformToMat4(traf0);
		//Transform traf0000 = mat4ToTransform(mat0);

		glm::mat4 mat1 = transformToMat4(traf1);
		//GLMTransform traf1111 = mat4ToTransform(mat1);

		expect_mat4_eq(mat0, mat1);
	}
}