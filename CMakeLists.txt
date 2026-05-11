cmake_minimum_required(VERSION 4.1.2)
project(FoPTransApp_Final)

# Silence the OpenGL warning by preferring GLVND
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(CppUTest CONFIG REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(lib/imgui-1.92.7)

add_executable(FoPTransApp_Final
        src/main.cpp
        src/files/LocPackFile.cpp
        src/files/LocPackBinFile.cpp
        src/utils/DataSerializer.cpp
        src/utils/LocaleLine.cpp
        src/files/FileManager.cpp
        src/ui/MainUI.cpp
        src/ui/MenuBar.cpp
        src/ui/LocaleTable.cpp
)

target_include_directories(FoPTransApp_Final PUBLIC
        include
        lib/imgui-1.92.7
        lib/imgui-1.92.7/backends
)

target_link_libraries(FoPTransApp_Final PRIVATE
        imgui
        nlohmann_json::nlohmann_json
        CppUTest
        rapidcsv::rapidcsv
        glfw
        OpenGL::GL
)