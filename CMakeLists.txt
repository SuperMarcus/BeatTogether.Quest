cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER_TARGET aarch64-none-linux-android24)
set(CMAKE_C_COMPILER $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang)
set(CMAKE_CXX_COMPILER_TARGET aarch64-none-linux-android24)
set(CMAKE_CXX_COMPILER $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++)

project(questmod)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_SYSROOT /)
set(CMAKE_OSX_DEPLOYMENT_TARGET "")

add_compile_definitions(VERSION='"dummy_version"' ID='"dummy"' HOST_NAME="example.com" PORT='1234')

include_directories(include)
include_directories(extern)
include_directories(shared)
include_directories(extern/libil2cpp/il2cpp/libil2cpp)
include_directories(extern/codegen/include)
include_directories(extern/beatsaber-hook/shared)
include_directories(extern/questui/shared)
include_directories(extern/modloader/shared)
include_directories($ENV{ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++/include)
include_directories($ENV{ANDROID_NDK_HOME}/sysroot/usr/include)
include_directories($ENV{ANDROID_NDK_HOME}/sources/cxx-stl/llvm-libc++abi/include)
include_directories($ENV{ANDROID_NDK_HOME}/sysroot/usr/include/arm-linux-androideabi)

file(GLOB SRC src/*.cpp)
add_executable(questmod ${SRC})
