
# Add an application to play around with
set(

    ChoreoGrapherSources
    ChoreoGrapher.cpp
    )


include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoApp)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/imgui/)
link_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoApp/)

add_executable(
    ChoreoGrapher ${ChoreoGrapherSources} 
)

target_compile_options(ChoreoGrapher PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
target_compile_features(ChoreoGrapher PUBLIC cxx_std_17)
target_link_libraries(ChoreoGrapher PUBLIC imgui glm ChoreoApp)

