cmake_minimum_required(VERSION 3.5)

project(tg_sign_1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_executable(tg_sign_1
    main.cpp
    verify.cpp
    )
target_link_libraries(tg_sign_1 Qt5::Core)
