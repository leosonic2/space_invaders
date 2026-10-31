cmake_minimum_required(VERSION 3.21)

project(space_invaders VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
foreach(configuration DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${configuration} "${CMAKE_BINARY_DIR}/bin")
endforeach()

set(SPACE_INVADERS_FINAL_WAVE 5 CACHE STRING "Final wave in the campaign")
if(NOT SPACE_INVADERS_FINAL_WAVE MATCHES "^[1-9][0-9]*$")
    message(FATAL_ERROR "SPACE_INVADERS_FINAL_WAVE must be a positive integer")
endif()
message(STATUS "Space Invaders campaign final wave: ${SPACE_INVADERS_FINAL_WAVE}")

include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw glad)

add_subdirectory("${glad_SOURCE_DIR}/cmake" "${glad_BINARY_DIR}")
glad_add_library(glad_gl_core_33 STATIC REPRODUCIBLE API gl:core=3.3)

configure_file(
    include/game_config.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/generated/game_config.h"
    @ONLY
)

add_library(game_core STATIC src/game.c)
target_include_directories(game_core PUBLIC
    include
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

add_library(game_renderer STATIC src/renderer.c)
target_include_directories(game_renderer PUBLIC include)
target_link_libraries(game_renderer PUBLIC glad_gl_core_33)

add_executable(space_invaders src/main.c)
target_include_directories(space_invaders PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
target_link_libraries(space_invaders PRIVATE game_core game_renderer glfw glad_gl_core_33)
if(TARGET OpenGL::GL)
    target_link_libraries(space_invaders PRIVATE OpenGL::GL)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(space_invaders PRIVATE OpenGL::GL)
endif()

if(MSVC)
    target_compile_options(space_invaders PRIVATE /W4)
else()
    target_compile_options(space_invaders PRIVATE -Wall -Wextra -Wpedantic)
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders")
configure_file(shaders/vertex.glsl "${CMAKE_CURRENT_BINARY_DIR}/shaders/vertex.glsl" COPYONLY)
configure_file(shaders/fragment.glsl "${CMAKE_CURRENT_BINARY_DIR}/shaders/fragment.glsl" COPYONLY)

include(CTest)
if(BUILD_TESTING)
    add_executable(test_configuration tests/test_configuration.c)
    target_include_directories(test_configuration PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
    )
    target_compile_definitions(test_configuration PRIVATE
        EXPECTED_FINAL_WAVE=${SPACE_INVADERS_FINAL_WAVE}
    )
    add_test(NAME campaign_configuration COMMAND test_configuration)

    add_executable(test_game tests/test_game.c)
    target_link_libraries(test_game PRIVATE game_core)
    add_test(NAME game_logic COMMAND test_game)

    add_executable(test_renderer tests/test_renderer.c)
    target_link_libraries(test_renderer PRIVATE game_renderer)
    add_test(NAME renderer_viewport COMMAND test_renderer)

    add_executable(test_renderer_text tests/test_renderer_text.c)
    target_link_libraries(test_renderer_text PRIVATE game_renderer)
    add_test(NAME renderer_text COMMAND test_renderer_text)

    add_executable(test_integration tests/test_integration.c)
    target_link_libraries(test_integration PRIVATE game_core)
    add_test(NAME game_integration COMMAND test_integration)
endif()
