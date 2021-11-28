#pragma warning(disable : 26451)
#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
#include "GLTFLoader.h"
#include "Uniform.h"
#include <GL/glew.h>

void Sample::Initialize(const std::string& rootPath) {
	//cgltf_data* gltf = LoadGLTFFile((rootPath + "Assets/robot.gltf").c_str());
	cgltf_data* gltf = LoadGLTFFile((rootPath + "Assets/knight.gltf").c_str());
	LoadMeshes(mCPUMeshes, gltf);
	mSkeleton = LoadSkeleton(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	mGPUMeshes = mCPUMeshes;
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].UpdateOpenGLBuffers();
	}

	mStaticShader = new Shader(rootPath + "Shaders/static.vert", rootPath + "Shaders/lit.frag");
	mSkinnedShader = new Shader(rootPath + "Shaders/skinned.vert", rootPath + "Shaders/lit.frag");
	//mDiffuseTexture = new Texture((rootPath + "Assets/robot_basecolor.png").c_str());
	mDiffuseTexture = new Texture((rootPath + "Assets/knight_basecolor.png").c_str());

	mGPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());
	mCPUAnimInfo.mAnimatedPose = mSkeleton.GetRestPose();
	mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetRestPose().Size());

	mGPUAnimInfo.mModel.position = glm::vec3(-0.75, 0, 0);
	mCPUAnimInfo.mModel.position = glm::vec3(0.75, 0, 0);

	mCPUCurrentPoseVisual = new DebugDraw();
	mCPUCurrentPoseVisual->FromPose(mSkeleton.GetRestPose());
	mCPUCurrentPoseVisual->UpdateOpenGLBuffers();

	mGPUCurrentPoseVisual = new DebugDraw();
	mGPUCurrentPoseVisual->FromPose(mSkeleton.GetRestPose());
	mGPUCurrentPoseVisual->UpdateOpenGLBuffers();

	unsigned int numUIClips = (unsigned int)mClips.size();
	for (unsigned int i = 0; i < numUIClips; ++i) {
		if (mClips[i].GetName() == "run") {
			mCPUAnimInfo.mClip = i;
		}
		else if (mClips[i].GetName() == "powerup") {
			mGPUAnimInfo.mClip = i;
		}
	}
}

void Sample::Update(float deltaTime) {
	mCPUAnimInfo.mPlayback = mClips[mCPUAnimInfo.mClip].Sample(mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mPlayback + deltaTime);
	mGPUAnimInfo.mPlayback = mClips[mGPUAnimInfo.mClip].Sample(mGPUAnimInfo.mAnimatedPose, mGPUAnimInfo.mPlayback + deltaTime);

	mCPUCurrentPoseVisual->FromPose(mCPUAnimInfo.mAnimatedPose);
	mGPUCurrentPoseVisual->FromPose(mGPUAnimInfo.mAnimatedPose);
	
	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].CPUSkin(mSkeleton, mCPUAnimInfo.mAnimatedPose);
	}

	mGPUAnimInfo.mAnimatedPose.GetMatrixPalette(mGPUAnimInfo.mPosePalette);
}

void Sample::Render(float inAspectRatio) {
	glm::mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f + 0.1f, 2.5f), glm::vec3(0.0f, 0.75f + 0.1f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model;

	// CPU Skinned Mesh
	model = transformToMat4(mCPUAnimInfo.mModel);
	mStaticShader->Bind();
	Uniform<glm::mat4>::Set(mStaticShader->GetUniform("model"), model);
	Uniform<glm::mat4>::Set(mStaticShader->GetUniform("view"), view);
	Uniform<glm::mat4>::Set(mStaticShader->GetUniform("projection"), projection);
	Uniform<glm::vec3>::Set(mStaticShader->GetUniform("light"), glm::vec3(1, 1, 1));

	mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
		mCPUMeshes[i].Draw();
		mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
	}
	mDiffuseTexture->UnSet(0);
	mStaticShader->UnBind();

	// CPU Skinned Mesh Debug
	model[3][0] += 1.25;
	mCPUCurrentPoseVisual->UpdateOpenGLBuffers();
	mCPUCurrentPoseVisual->Draw(DebugDrawMode::Lines, glm::vec3(0, 0, 1), projection * view * model);

	// GPU Skinned Mesh
	model = transformToMat4(mGPUAnimInfo.mModel);
	mSkinnedShader->Bind();
	Uniform<glm::mat4>::Set(mSkinnedShader->GetUniform("model"), model);
	Uniform<glm::mat4>::Set(mSkinnedShader->GetUniform("view"), view);
	Uniform<glm::mat4>::Set(mSkinnedShader->GetUniform("projection"), projection);
	Uniform<glm::vec3>::Set(mSkinnedShader->GetUniform("light"), glm::vec3(1, 1, 1));

	Uniform<glm::mat4>::Set(mSkinnedShader->GetUniform("pose"), mGPUAnimInfo.mPosePalette);
	Uniform<glm::mat4>::Set(mSkinnedShader->GetUniform("invBindPose"), mSkeleton.GetInvBindPose());

	mDiffuseTexture->Set(mSkinnedShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].Bind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
		mGPUMeshes[i].Draw();
		mGPUMeshes[i].UnBind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
	}
	mDiffuseTexture->UnSet(0);
	mSkinnedShader->UnBind();

	// GPU Skinned Mesh Debug
	model[3][0] += -1.25;
	mGPUCurrentPoseVisual->UpdateOpenGLBuffers();
	mGPUCurrentPoseVisual->Draw(DebugDrawMode::Lines, glm::vec3(0, 0, 1), projection * view * model);
}

void Sample::Shutdown() {
	delete mStaticShader;
	delete mDiffuseTexture;
	delete mSkinnedShader;
	mClips.clear();
	mCPUMeshes.clear();
	mGPUMeshes.clear();

	delete mCPUCurrentPoseVisual;
	delete mGPUCurrentPoseVisual;
}