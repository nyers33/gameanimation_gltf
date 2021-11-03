#ifndef _H_GLTFLOADER_
#define _H_GLTFLOADER_

#include "Pose.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "Clip.h"
#include <vector>
#include <string>

struct cgltf_data;

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* handle);

Pose LoadRestPose(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);
Pose LoadBindPose(cgltf_data* data);
Skeleton LoadSkeleton(cgltf_data* data);
void LoadMeshes(std::vector<Mesh>&, cgltf_data* data);

#endif
