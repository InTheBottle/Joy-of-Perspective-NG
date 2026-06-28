cmake_minimum_required(VERSION 3.21)
project(JoP-Patch VERSION 1.0.0 DESCRIPTION "JoP 1st-person mesh redirect" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Build CommonLibSSE-NG from source via the extern/ submodule instead of a prebuilt vcpkg port.
# All three runtimes ON -> a single DLL that loads on Skyrim SE (1.5.x), AE (1.6.x), and VR.
set(BUILD_TESTS OFF)       # skip CommonLibSSE's own Catch2 test suite
set(ENABLE_SKYRIM_SE ON)
set(ENABLE_SKYRIM_AE ON)
set(ENABLE_SKYRIM_VR ON)
add_subdirectory(extern/CommonLibSSE-NG)

add_library(${PROJECT_NAME} SHARED src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSE::CommonLibSSE)

install(TARGETS ${PROJECT_NAME}
    DESTINATION "Data/SKSE/Plugins")
