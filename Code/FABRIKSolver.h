#ifndef _H_FABRIKSOLVER_
#define _H_FABRIKSOLVER_

#include "Transform.h"
#include <vector>

class FABRIKSolver {
protected:
	std::vector<Transform> mIKChain;
	std::vector<glm::vec3> mWorldChain;
	std::vector<float> mLengths;
	unsigned int mNumSteps;
	float mThreshold;
protected:
	void IKChainToWorld();
	void IterateForward(const glm::vec3& goal);
	void IterateBackward(const glm::vec3& base);
	void WorldToIKChain();
public:
	FABRIKSolver();

	unsigned int Size();
	void Resize(unsigned int newSize);
	Transform GetLocalTransform(unsigned int index);
	void SetLocalTransform(unsigned int index, const Transform& t);
	Transform GetGlobalTransform(unsigned int index);

	unsigned int GetNumSteps();
	void SetNumSteps(unsigned int numSteps);

	float GetThreshold();
	void SetThreshold(float value);

	bool Solve(const Transform& target);
	bool Solve(const glm::vec3& target);
};

#endif