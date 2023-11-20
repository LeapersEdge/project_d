cmake_minimum_required(VERSION 3.19.2)

project(project_d VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)

# set a path for third-party dir
set(THIRD_PARTY "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

# Add raylib library
add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES IMPORTED_LOCATION "${THIRD_PARTY}/raylib/libraylib.a")

# Your existing code for sources
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(project_d "${MY_SOURCES}")

# Your existing code for include directory
target_include_directories(project_d PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${THIRD_PARTY}"
    "${THIRD_PARTY}/raylib/include"
    "${THIRD_PARTY}/raygui/include"
    "${THIRD_PARTY}/raylib-cpp/include"
)

# Your existing code for resources path
target_compile_definitions(project_d PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

# Link against raylib, raygui, and static_libs
target_link_libraries(project_d PRIVATE raylib)
