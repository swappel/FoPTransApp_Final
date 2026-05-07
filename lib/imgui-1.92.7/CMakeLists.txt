project(imgui_library)

file(GLOB IMGUI_SOURCES "*.cpp")

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backends)