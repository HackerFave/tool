cmake_minimum_required(VERSION 3.5)

project(tg_sign LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

#include_directories(/home/tc/work/openssl/build/include/openssl)
#link_directories(/home/tc/work/openssl/build/lib)
include_directories(/usr/include)
link_directories(/usr/lib)
add_executable(tg_sign
  main.cpp
)
target_link_libraries(tg_sign Qt5::Core)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} crypto ssl)
