
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(spdlog REQUIRED)
# find_package(Assimp REQUIRED)
# include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${ChoreoFlocking_SOURCE_DIR}/src/ChoreoFlocking/include)
include_directories(${ChoreoFlocking_SOURCE_DIR}/src/ChoreoFlocking/ChoreoFlocking)
# link_directories(include)
# find_package( Boost 1.40 COMPONENTS program_options REQUIRED )

# Add the core lib
set(
    ChoreoFlockingSources 
    ChoreoFlocking/Application.cpp
    ChoreoFlocking/Log.cpp
    dependencies/glad.c
    dependencies/stb_image.cpp
    )
add_library(
    ChoreoFlocking ${ChoreoFlockingSources}
)
# target_compile_features(ChoreoFlocking PUBLIC cxx_std_17)
target_link_libraries(ChoreoFlocking PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARIES} spdlog::spdlog)
# target_compile_options(ChoreoFlocking PRIVATE
#     "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
#     )
set_target_properties(ChoreoFlocking PROPERTIES LINKER_LANGUAGE CXX)
