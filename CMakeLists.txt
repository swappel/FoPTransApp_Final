cmake_minimum_required(VERSION 4.2)
project(FoPTransApp_Final)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(CppUTest CONFIG REQUIRED)
find_package(FTXUI REQUIRED)
find_package(rapidcsv CONFIG REQUIRED)
find_package(fkYAML CONFIG REQUIRED)
find_package(libgit2 CONFIG REQUIRED)

add_executable(FoPTransApp_Final src/main.cpp
        src/utils/GitHubManager.cpp
        src/files/LocPackFile.cpp
        include/files/LocPackFile.h
        src/files/LocPackBinFile.cpp
        include/files/LocPackBinFile.h
        src/utils/DataSerializer.cpp
        include/utils/DataSerializer.h
        src/utils/LocaleLine.cpp
        include/utils/LocaleLine.h)

target_include_directories(FoPTransApp_Final PUBLIC include)
target_link_libraries(FoPTransApp_Final PRIVATE
        nlohmann_json::nlohmann_json
        CppUTest
        ftxui::screen
        ftxui::dom
        ftxui::component
        rapidcsv::rapidcsv
        fkYAML::fkYAML
        libgit2::libgit2package
)