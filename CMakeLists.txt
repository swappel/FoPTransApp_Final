cmake_minimum_required(VERSION 4.1.2)
project(FoPTransApp_Final)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(CppUTest CONFIG REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)

add_subdirectory(lib/imgui)

add_executable(FoPTransApp_Final src/main.cpp
        src/files/LocPackFile.cpp
        include/files/LocPackFile.h
        src/files/LocPackBinFile.cpp
        include/files/LocPackBinFile.h
        src/utils/DataSerializer.cpp
        include/utils/DataSerializer.h
        src/utils/LocaleLine.cpp
        include/utils/LocaleLine.h
        include/files/FileManager.h
        src/files/FileManager.cpp
        include/ui/MainUI.h
        src/ui/MainUI.cpp)

target_link_libraries(FoPTransApp_Final PRIVATE imgui)

target_include_directories(FoPTransApp_Final PUBLIC include)
target_link_libraries(FoPTransApp_Final PRIVATE
        nlohmann_json::nlohmann_json
        CppUTest
        rapidcsv::rapidcsv
)