cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "/mnt/massive/Coding/CPP/FoPTransApp_Final/.vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(FoPTransApp_Final)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DNAPI_VERSION=4)

if (MSVC)
    add_compile_options(/EHsc)
else ()
    add_compile_options(-fexceptions)
endif ()

include(FetchContent)
FetchContent_Declare(
        node_addon_api
        URL https://github.com/nodejs/node-addon-api/archive/refs/tags/v8.9.2.tar.gz
)
FetchContent_MakeAvailable(node_addon_api)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)

# Build as a Shared Native Node Module (.node) instead of an Executable
add_library(${PROJECT_NAME} SHARED
        src/files/LocPackFile.cpp
        src/files/LocPackBinFile.cpp
        src/utils/LocaleLine.cpp
        src/files/FileManager.cpp
        src/utils/addon.cpp
        src/utils/Backend.cpp
        src/main.cpp
        src/utils/LineSearchResult.cpp
        include/utils/LineSearchResult.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${node_addon_api_SOURCE_DIR}
        ${CMAKE_JS_INC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        nlohmann_json::nlohmann_json
        rapidcsv::rapidcsv
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        NAPI_DISABLE_CPP_EXCEPTIONS
        BUILDING_NODE_EXTENSION
)

# Set naming convention for Electron / Node.js
set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        SUFFIX ".node"
)