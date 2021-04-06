
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(spdlog REQUIRED)
# find_package(Assimp REQUIRED)
# include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/ChoreoEngine)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoEngine/)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/glad/include/)
# link_directories(include)
# find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

# Add the core lib
set(
    ChoreoEngineSources 
    ChoreoEngine/Application.cpp
    ChoreoEngine/Layer.cpp
    ChoreoEngine/LayerStack.cpp
    ChoreoEngine/Platform/Linux/LinuxWindow.cpp
    ChoreoEngine/Log.cpp
    )
add_library(
    ChoreoEngine ${ChoreoEngineSources}
)

target_precompile_headers(ChoreoEngine 
    PRIVATE
    "cepch.h"
    )
target_compile_features(ChoreoEngine PUBLIC cxx_std_17)
target_link_libraries(ChoreoEngine PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES} spdlog::spdlog PRIVATE glad)
target_compile_options(ChoreoEngine PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )

