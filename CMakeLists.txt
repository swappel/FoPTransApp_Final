cmake_minimum_required(VERSION 3.20)

project(FoPTransApp_Final LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(NAPI_VERSION=4)

if(MSVC)
    add_compile_options(/EHsc)
else()
    add_compile_options(-fexceptions)
endif()

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        rapidcsv
        URL https://github.com/d99kris/rapidcsv/archive/refs/heads/master.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(rapidcsv)

FetchContent_Declare(
        node_addon_api
        URL https://github.com/nodejs/node-addon-api/archive/refs/tags/v8.9.2.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(node_addon_api)

set(ADDON_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/files/LocPackFile.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/files/LocPackBinFile.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/LocaleLine.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/files/FileManager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/addon.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Backend.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/SearchEngine.cpp"
)

if(WIN32 AND DEFINED CMAKE_JS_SRC AND EXISTS "${CMAKE_JS_SRC}")
    list(APPEND ADDON_SOURCES "${CMAKE_JS_SRC}")
endif()

add_library(${PROJECT_NAME} SHARED
        ${ADDON_SOURCES}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${node_addon_api_SOURCE_DIR}"
        "${rapidcsv_SOURCE_DIR}/src"
)

if(DEFINED CMAKE_JS_INC)
    target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${CMAKE_JS_INC}
    )
endif()

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
)

if(DEFINED CMAKE_JS_LIB)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            ${CMAKE_JS_LIB}
    )
endif()

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        BUILDING_NODE_EXTENSION
        NAPI_DISABLE_CPP_EXCEPTIONS
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            delayimp
    )

    if(NOT DEFINED CMAKE_SHARED_LINKER_FLAGS
            OR NOT CMAKE_SHARED_LINKER_FLAGS MATCHES "DELAYLOAD")
        target_link_options(${PROJECT_NAME}
                PRIVATE
                "/DELAYLOAD:NODE.EXE"
        )
    endif()
endif()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PREFIX ""
        SUFFIX ".node"
)
