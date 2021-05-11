
# Add an application to play around with
set(
    ChoreoGrapherSources
    ChoreoGrapher.cpp

    Ui/EditorLayer.cpp

    Ui/Panels/SceneHierarchyPanel.cpp
    Ui/Panels/EntityPropertiesPanel.cpp
    Ui/Panels/ControllorEditorPanel.cpp
    Ui/Panels/TimelinePanel.cpp
    Ui/Widgets/ControllerEdit.cpp
    Ui/Widgets/CurveEditor.cpp
    Ui/Widgets/Timeline.cpp
)

message(${CMAKE_CURRENT_SOURCE_DIR}/Application)

include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoApp)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoGrapher)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/ChoreoGrapher/Application)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/imgui/)
include_directories(${ChoreoEngine_SOURCE_DIR}/src/dependencies/entt/)

add_executable(
    ChoreoGrapher ${ChoreoGrapherSources} 
)

target_compile_options(ChoreoGrapher PRIVATE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:${gcc_flags}>>" "$<${msvc_cxx}: $<BUILD_INTERFACE:${msvc_flags}>>"
    )
target_compile_features(ChoreoGrapher PUBLIC cxx_std_17)
target_link_libraries(ChoreoGrapher PUBLIC imgui glm ChoreoApp)

