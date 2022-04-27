cmake_minimum_required(VERSION 3.5)

project(sign_xinchangcheng_x64 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLS_ROOT ${CMAKE_PROJECT_ROOT}/lib)
set(CMAKE_FRAMEWORK_ROOT ${CMAKE_PROJECT_ROOT}/include)

include_directories(./include)
link_directories(./lib)
link_directories(/usr/local/lib)

# System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS
    thread
    )

if(NOT Boost_FOUND)
    message("NOT found Boost")
endif()
#find_package(Qt5Core)

add_executable(sign_xinchangcheng_x64
    main.cpp
    httpMethord.cpp
    signSdk.cpp
    client_ssl.cpp
    mq_client.cpp
    )
#target_link_libraries(sign_xinchangcheng_x64 Qt5::Core)
target_link_libraries(${PROJECT_NAME} gmscm jsoncpp pthread crypto rabbitmq SimpleAmqpClient)
#target_link_libraries(${PROJECT_NAME} gmscm pthread)
