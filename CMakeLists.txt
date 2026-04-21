cmake_minimum_required(VERSION 4.2)
project(FoPTransApp_Final)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json REQUIRED)
find_package(CppUTest REQUIRED)
find_package(FTXUI REQUIRED)

add_executable(FoPTransApp_Final src/main.cpp
        src/utils/GitHubManager.cpp
        src/files/LocPackFile.cpp
        include/files/LocPackFile.h
        src/files/LocPackBinFile.cpp
        include/files/LocPackBinFile.h
        src/utils/DataSerializer.cpp
        include/utils/DataSerializer.h)

target_include_directories(FoPTransApp_Final PUBLIC include)
target_link_libraries(FoPTransApp_Final PRIVATE
        nlohmann_json::nlohmann_json
        CppUTest
        ftxui::screen
        ftxui::dom
        ftxui::component
)