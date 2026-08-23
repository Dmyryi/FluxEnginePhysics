cmake_minimum_required(VERSION 3.24)
project(FluxEnginePhysics CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)

# 1. SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)

# 2. ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
)

# 3. ImGui-SFML
set(IMGUI_SFML_BUILD_STATIC ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v2.6
)

FetchContent_MakeAvailable(SFML imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_MakeAvailable(ImGui-SFML)

set_target_properties(sfml-graphics sfml-window sfml-system sfml-audio sfml-network ImGui-SFML PROPERTIES FOLDER "Dependencies")

# ==========================================
# 1. FluxCore (берем из папки FluxCore)
# ==========================================
file(GLOB_RECURSE CORE_SOURCES "FluxCore/*.*")
add_library(FluxCore STATIC ${CORE_SOURCES})
target_include_directories(FluxCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/FluxCore")

# ==========================================
# 2. FluxPresentation (берем из папки FluxPresentation)
# ==========================================
file(GLOB_RECURSE PRES_SOURCES "FluxPresentation/*.*")
add_library(FluxPresentation STATIC ${PRES_SOURCES})
target_include_directories(FluxPresentation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/FluxPresentation")
target_link_libraries(FluxPresentation PUBLIC 
    FluxCore 
    sfml-graphics 
    sfml-window 
    sfml-system 
    ImGui-SFML::ImGui-SFML
)

# ==========================================
# 3. Главный исполняемый проект
# ==========================================
add_executable(FluxEnginePhysics 
    src/main.cpp
)

target_link_libraries(FluxEnginePhysics PRIVATE 
    FluxCore 
    FluxPresentation 
    sfml-graphics 
    sfml-window 
    sfml-system 
    ImGui-SFML::ImGui-SFML
)

if (WIN32)
    add_custom_command(TARGET FluxEnginePhysics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:sfml-graphics>
        $<TARGET_FILE_DIR:FluxEnginePhysics>
    )
endif()