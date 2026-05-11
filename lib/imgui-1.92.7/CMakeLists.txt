project(imgui_library)

file(GLOB IMGUI_SOURCES "*.cpp")

list(APPEND IMGUI_SOURCES
        "backends/imgui_impl_glfw.cpp"
        "backends/imgui_impl_opengl3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
)