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
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCE_DEBUG -DCE_ENABLE_ASSERTS ")
    message("Building in Debug mode")
ELSE()
    message("Building in Release mode")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
# uncomment the line below to enable profiling.
# CE will write out json logs that you can load into chrome://trace
# add_compile_definitions(CE_PROFILE)
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
add_subdirectory(src/ChoreoApp)

add_subdirectory(src/ChoreoGrapher)
add_dependencies(ChoreoGrapher ChoreoApp)


add_subdirectory(src/Sandbox)
add_dependencies(Sandbox ChoreoApp)


add_custom_target(
    copy_shader_files
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/shaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
    COMMENT "Copying shader files" VERBATIM
    )

add_dependencies( ChoreoApp copy_shader_files )
add_dependencies( ChoreoGrapher copy_shader_files )
add_dependencies( Sandbox copy_shader_files )

# add custom target to copy texture files across

add_custom_target(
    copy_texture_files 
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/textures
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures
    COMMENT "Copying texture files" VERBATIM
    )
# ChoreoEngine doesn't need textures
add_dependencies( ChoreoGrapher copy_texture_files )
add_dependencies( Sandbox copy_texture_files )

# add custom target to copy font files across
add_custom_target(
    copy_font_files 
    ${CMAKE_COMMAND}
    -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/fonts
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/fonts
    COMMENT "Copying font files" VERBATIM
    )

add_dependencies(ChoreoApp copy_font_files)
add_dependencies(ChoreoGrapher copy_font_files)
