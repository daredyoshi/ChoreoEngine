cmake_minimum_required (VERSION 3.16) 
project (ChoreoEngine VERSION 0.0.1 LANGUAGES CXX)
# This is so Ycm can find and complete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add compiler warning flags just when building this project
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
# for unused variables
# set(gcc_flags "-Wall;-Werror;-Wunused;-Wextra;-Wshadow;-Wshadow;-Wformat=2;-pedantic-errors;-std=c++17")

set(gcc_flags "-Wall;-Werror;-Wunused;-Wno-unused-variable;-Wextra;-Wshadow;-Wshadow;-Wformat=2;-pedantic-errors;-std=c++17")
set(msvc_flags "-W3")

# set up a Choreo Debug variable
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCE_DEBUG -DCE_ENABLE_ASSERTS -DCE_PROFILE")
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

# ideally you would set these cmake
set (BUILD_SHARED_LIBS ON)
set (CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# At first I tried building glad with ChoreoEngine, but then it 
# wasn't linking. Seems nice to have it as it's own lib anyway :P
add_subdirectory(src/dependencies/glad)
add_subdirectory(src/dependencies/imgui)
add_subdirectory(src/dependencies/glm)
add_subdirectory(src/ChoreoEngine)
add_subdirectory(src/Sandbox)



