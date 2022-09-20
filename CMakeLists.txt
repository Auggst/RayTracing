cmake_minimum_required(VERSION 3.18)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
include_directories(external images include src)

file(GLOB_RECURSE native_srcs src/*.cpp)
file(GLOB_RECURSE native_include include/.h)

#vec3.h ray.h sphere.h sphere.cpp myutils.h hittable.h hittable.cpp scene.h scene.cpp camera.h camera.cpp material.h material.cpp bvh.h bvh.cpp aabb.h aabb.cpp texture.h texture.cpp perlin.h perlin.cpp aarect.h aarect.cpp box.h box.cpp constant_medium.h constant_medium.cpp
add_executable(RayTracing main.cc ${native_include} ${native_srcs})
target_compile_options(RayTracing PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -fsanitize=undefined)
target_compile_features(RayTracing PUBLIC cxx_std_17)
target_link_libraries(RayTracing PUBLIC -fsanitize=undefined)
