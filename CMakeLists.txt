cmake_minimum_required(VERSION 4.1.2)
project(FoPTransApp_Final)

# Silence the OpenGL warning by preferring GLVND
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        node_addon_api
        URL https://github.com/nodejs/node-addon-api/archive/refs/tags/v8.9.2.tar.gz
)

FetchContent_MakeAvailable(node_addon_api)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(CppUTest CONFIG REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(FoPTransApp_Final
        src/main.cpp
        src/files/LocPackFile.cpp
        src/files/LocPackBinFile.cpp
        src/utils/DataSerializer.cpp
        src/utils/LocaleLine.cpp
        src/files/FileManager.cpp
        src/utils/addon.cpp
)

target_include_directories(FoPTransApp_Final PRIVATE
        include
        ${node_addon_api_SOURCE_DIR}
)

target_link_libraries(FoPTransApp_Final PRIVATE
        nlohmann_json::nlohmann_json
        CppUTest
        rapidcsv::rapidcsv
        glfw
        OpenGL::GL
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        NAPI_DISABLE_CPP_EXCEPTIONS
)