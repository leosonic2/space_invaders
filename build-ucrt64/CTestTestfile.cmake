# CMake generated Testfile for 
# Source directory: C:/estudos/space_invaders
# Build directory: C:/estudos/space_invaders/build-ucrt64
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test("campaign_configuration" "C:/estudos/space_invaders/build-ucrt64/bin/test_configuration.exe")
set_tests_properties("campaign_configuration" PROPERTIES  _BACKTRACE_TRIPLES "C:/estudos/space_invaders/CMakeLists.txt;92;add_test;C:/estudos/space_invaders/CMakeLists.txt;0;")
add_test("game_logic" "C:/estudos/space_invaders/build-ucrt64/bin/test_game.exe")
set_tests_properties("game_logic" PROPERTIES  _BACKTRACE_TRIPLES "C:/estudos/space_invaders/CMakeLists.txt;96;add_test;C:/estudos/space_invaders/CMakeLists.txt;0;")
add_test("renderer_viewport" "C:/estudos/space_invaders/build-ucrt64/bin/test_renderer.exe")
set_tests_properties("renderer_viewport" PROPERTIES  _BACKTRACE_TRIPLES "C:/estudos/space_invaders/CMakeLists.txt;100;add_test;C:/estudos/space_invaders/CMakeLists.txt;0;")
add_test("renderer_text" "C:/estudos/space_invaders/build-ucrt64/bin/test_renderer_text.exe")
set_tests_properties("renderer_text" PROPERTIES  _BACKTRACE_TRIPLES "C:/estudos/space_invaders/CMakeLists.txt;104;add_test;C:/estudos/space_invaders/CMakeLists.txt;0;")
add_test("game_integration" "C:/estudos/space_invaders/build-ucrt64/bin/test_integration.exe")
set_tests_properties("game_integration" PROPERTIES  _BACKTRACE_TRIPLES "C:/estudos/space_invaders/CMakeLists.txt;108;add_test;C:/estudos/space_invaders/CMakeLists.txt;0;")
subdirs("_deps/glfw-build")
subdirs("_deps/glad-build")
