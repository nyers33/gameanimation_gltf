#ifndef _H_MESH_
#define _H_MESH_

#include <vector>
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Skeleton.h"
#include "Pose.h"

class Mesh {
protected:
	std::vector<glm::vec3> mPosition;
	std::vector<glm::vec3> mNormal;
	std::vector<glm::vec2> mTexCoord;
	std::vector<glm::vec4> mWeights;
	std::vector<glm::ivec4> mInfluences;
	std::vector<unsigned int> mIndices;
protected:
	Attribute<glm::vec3>* mPosAttrib;
	Attribute<glm::vec3>* mNormAttrib;
	Attribute<glm::vec2>* mUvAttrib;
	Attribute<glm::vec4>* mWeightAttrib;
	Attribute<glm::ivec4>* mInfluenceAttrib;
	IndexBuffer* mIndexBuffer;
protected:
	std::vector<glm::vec3> mSkinnedPosition;
	std::vector<glm::vec3> mSkinnedNormal;
	std::vector<glm::mat4> mPosePalette;
public:
	Mesh();
	Mesh(const Mesh&);
	Mesh& operator=(const Mesh&);
	~Mesh();
	std::vector<glm::vec3>& GetPosition();
	std::vector<glm::vec3>& GetNormal();
	std::vector<glm::vec2>& GetTexCoord();
	std::vector<glm::vec4>& GetWeights();
	std::vector<glm::ivec4>& GetInfluences();
	std::vector<unsigned int>& GetIndices();
	void CPUSkin(Skeleton& skeleton, Pose& pose);
	void UpdateOpenGLBuffers();
	void Bind(int position, int normal, int texCoord, int weight, int influcence);
	void Draw();
	void DrawInstanced(unsigned int numInstances);
	void UnBind(int position, int normal, int texCoord, int weight, int influcence);
};


#endif // !_H_MESH_
