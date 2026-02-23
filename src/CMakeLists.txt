# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")

idf_component_register(
    SRCS 
        ${app_sources}
    INCLUDE_DIRS 
        "include"     # Pour trouver Globals.h
        "PumpState"   # Pour trouver PumpState.h
        "SystemState" # Pour trouver SystemState.h
        "Ui"          # Pour tes futurs fichiers UI
        "Assets"      # Pour que LVGL trouve les déclarations d'images
        "Assets/Font"
        "."           # Pour main.cpp
)