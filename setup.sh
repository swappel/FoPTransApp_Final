#!/bin/bash

git clone https://github.com/microsoft/vcpkg.git

./vcpkg/bootstrap.sh

vcpkg install nlohmann-json cpputest ftxui vcpkg-cmake-config vcpkg-cmake