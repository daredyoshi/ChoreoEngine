cmake_minimum_required (VERSION 3.14) 
project (ChoreoFlocking VERSION 1.0)
# This is so Ycm can find and complete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# add compiler warning flags just when building this project
# via the BUILD_INTERFACE generator expression
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(gcc_flags "-Wall;-Werror;-Wunused;-Wextra;-Wshadow;-Wshadow;-Wformat=2;-pedantic-errors;-std=c++17")
set(msvc_flags "-W3")


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
# find_package(Assimp REQUIRED)
# include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(include)
link_directories(include)
# find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

# Add the core lib
set(
    ChoreoFlockingSources 
    src/ChoreoFlocking/Application.cpp
    src/ChoreoFlocking/Application.h
    )
add_library(
    ChoreoFlocking STATIC ${ChoreoFlockingSources}
)
# target_compile_features(ChoreoFlocking PUBLIC cxx_std_17)
# target_link_libraries(ChoreoFlocking PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES})
# target_link_libraries(ChoreoFlocking PUBLIC glfw)
# target_compile_options(ChoreoFlocking PRIVATE
#     "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
#     )
set_target_properties(ChoreoFlocking PROPERTIES LINKER_LANGUAGE CXX)


# Add an application to play around with
set(
    SandboxSources
    src/Sandbox/Main.cpp
    )
add_executable(
    Sandbox ${SandboxSources} 
)
target_compile_features(Sandbox PUBLIC cxx_std_17)
target_link_libraries(Sandbox PUBLIC ChoreoFlocking)
target_compile_options(Sandbox PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )



