set(APP_NAME gameanimation_gltf)

set(SRC_MATH
	vec2.h
	vec4.h
	vec3.h
	vec3.cpp

	mat4.h
	mat4.cpp
	quat.h
	quat.cpp

	Transform.h
	Transform.cpp
)

set(SRC_ANIMATION
	Frame.h
	Interpolation.h

	Clip.h
	Clip.cpp
	Pose.h
	Pose.cpp
	Skeleton.h
	Skeleton.cpp
	Track.h
	Track.cpp
	TransformTrack.h
	TransformTrack.cpp

	CCDSolver.h
	CCDSolver.cpp
	FABRIKSolver.h
	FABRIKSolver.cpp
)

set(SRC_RENDERING
	Mesh.h
	Mesh.cpp
)

set(SRC_OGL
	Attribute.h
	Attribute.cpp
	Draw.h
	Draw.cpp
	DebugDraw.h
	DebugDraw.cpp
	IndexBuffer.h
	IndexBuffer.cpp
	Shader.h
	Shader.cpp
	Texture.h
	Texture.cpp
	Uniform.h
	Uniform.cpp

	stb_image.h
	stb_image.cpp
)

set(SRC_GLTF
	cgltf.h
	cgltf.c

	GLTFLoader.h
	GLTFLoader.cpp
)

add_executable(${APP_NAME} main.cpp Sample.h Sample.cpp ${SRC_MATH} ${SRC_ANIMATION} ${SRC_RENDERING} ${SRC_OGL} ${SRC_GLTF})
target_link_libraries(${APP_NAME} glfw glew_s OpenGL::GL)
target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${GLEW_INCLUDE_DIRS})

source_group("sources" FILES main.cpp Sample.h Sample.cpp)
source_group("sources\\Math" FILES ${SRC_MATH})
source_group("sources\\Animation" FILES ${SRC_ANIMATION})
source_group("sources\\Rendering" FILES ${SRC_RENDERING})
source_group("sources\\OpenGL" FILES ${SRC_OGL})
source_group("sources\\GLTF" FILES ${SRC_GLTF})

if(BUILD_TESTING)
	add_executable(glmmath main_glmmath.cpp GLMTransform.h GLMTransform.cpp ${SRC_MATH})
	
	target_link_libraries(glmmath glfw glew_s OpenGL::GL glm::glm)
	target_link_libraries(glmmath gtest_main)
	
	target_include_directories(glmmath PUBLIC ${CMAKE_CURRENT_LIST_DIR})
	
	source_group("sources" FILES main_glmmath.cpp GLMTransform.h GLMTransform.cpp)
	
	include(GoogleTest)
	gtest_discover_tests(glmmath)
endif()