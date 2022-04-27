cmake_minimum_required(VERSION 3.5)

project(sign_XinChangcheng LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5Core)
#include_directories(/home/tc/work/CGU/sign_XinChangcheng/include)
#link_directories(/home/tc/work/CGU/sign_XinChangcheng/lib)
#find_package(Qt5Core)
#include_directories(/home/tc/work/CGU/sign_xinchangcheng)
#include_directories(/home/tc/work/CGU/sign_xinchangcheng_x64/include)
#link_directories(/home/tc/work/CGU/sign_xinchangcheng/lib)
#include_directories(/home/tc/work/openssl/build/include/openssl)
#link_directories(/home/tc/work/openssl/build/lib)

# tools path
set(CMAKE_TOOLS_ROOT ${CMAKE_PROJECT_ROOT}/lib)
set(CMAKE_FRAMEWORK_ROOT ${CMAKE_PROJECT_ROOT}/include)

include_directories(./include)
link_directories(./lib)

# System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS
    thread
    )

if(NOT Boost_FOUND)
    message("NOT found Boost")
endif()

add_executable(sign_XinChangcheng
    main.cpp
    httpMethord.cpp
    signSdk.cpp
    client_ssl.cpp
    )
#target_link_libraries(sign_XinChangcheng Qt5::Core)
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} gmscm jsoncpp)
target_link_libraries(${PROJECT_NAME} gmscm pthread)
