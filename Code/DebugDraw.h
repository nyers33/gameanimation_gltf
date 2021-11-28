#ifndef _H_DEBUGLINE_
#define _H_DEBUGLINE_

#include "Shader.h"
#include "Attribute.h"
#include <vector>
#include "Pose.h"
#include "CCDSolver.h"
#include "FABRIKSolver.h"

enum class DebugDrawMode {
	Lines, Loop, Strip, Points
};

class DebugDraw {
protected:
	std::vector<glm::vec3> mPoints;
	Attribute<glm::vec3>* mAttribs;
	Shader* mShader;
private:
	DebugDraw(const DebugDraw&);
	DebugDraw& operator=(const DebugDraw&);
public:
	DebugDraw();
	DebugDraw(unsigned int size);
	~DebugDraw();

	unsigned int Size();
	void Resize(unsigned int newSize);
	glm::vec3& operator[](unsigned int index);
	void Push(const glm::vec3& v);

	void FromPose(Pose& pose);
	void LinesFromIKSolver(CCDSolver& solver);
	void PointsFromIKSolver(CCDSolver& solver);
	void LinesFromIKSolver(FABRIKSolver& solver);
	void PointsFromIKSolver(FABRIKSolver& solver);

	void UpdateOpenGLBuffers();
	void Draw(DebugDrawMode mode, const glm::vec3& color, const glm::mat4& mvp);
};

#endif