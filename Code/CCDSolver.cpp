#include "CCDSolver.h"
#include <iostream>

CCDSolver::CCDSolver() {
	mNumSteps = 15;
	mThreshold = 0.00001f;
}

unsigned int CCDSolver::Size() {
	return (unsigned int)mIKChain.size();
}

void CCDSolver::Resize(unsigned int newSize) {
	mIKChain.resize(newSize);
}

Transform& CCDSolver::operator[](unsigned int index) {
	return mIKChain[index];
}

Transform CCDSolver::GetLocalTransform(unsigned int index) {
	return mIKChain[index];
}

void CCDSolver::SetLocalTransform(unsigned int index, const Transform& t) {
	mIKChain[index] = t;
}

Transform CCDSolver::GetGlobalTransform(unsigned int index) {
	unsigned int size = (unsigned int)mIKChain.size();
	Transform world = mIKChain[index];
	for (int i = (int)index - 1; i >= 0; --i) {
		world = combine(mIKChain[i], world);
	}
	return world;
}

unsigned int CCDSolver::GetNumSteps() {
	return mNumSteps;
}

void CCDSolver::SetNumSteps(unsigned int numSteps) {
	mNumSteps = numSteps;
}


float CCDSolver::GetThreshold() {
	return mThreshold;
}

void CCDSolver::SetThreshold(float value) {
	mThreshold = value;
}

bool CCDSolver::Solve(const Transform& target) {
	return Solve(target.position);
}

bool CCDSolver::Solve(const glm::vec3& goal) {
	unsigned int size = Size();
	if (size == 0) { return false; }
	unsigned int last = size - 1;
	float thresholdSq = mThreshold * mThreshold;

	for (unsigned int i = 0; i < mNumSteps; ++i) {
		glm::vec3 effector = GetGlobalTransform(last).position;
		if (glm::length2(goal - effector) < thresholdSq) {
			return true;
		}

		for (int j = (int)size - 2; j >= 0; --j) {
			effector = GetGlobalTransform(last).position;

			Transform world = GetGlobalTransform(j);
			glm::vec3 position = world.position;
			glm::quat rotation = world.rotation;

			glm::vec3 toEffector = effector - position;
			glm::vec3 toGoal = goal - position;

			glm::quat effectorToGoal;
			if (glm::length2(toGoal) > 0.00001f) {
				effectorToGoal = fromTo(toEffector, toGoal);
			}

			glm::quat worldRotated = rotation * effectorToGoal;
			glm::quat localRotate = worldRotated * inverse(rotation);
			mIKChain[j].rotation = localRotate * mIKChain[j].rotation;

			effector = GetGlobalTransform(last).position;
			if (glm::length2(goal - effector) < thresholdSq) {
				return true;
			}
		}
	}

	glm::vec3 effector = GetGlobalTransform(last).position;
	if (glm::length2(goal - effector) < thresholdSq) {
		return true;
	}
	return false;
}