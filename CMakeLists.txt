cmake_minimum_required(VERSION 3.17)
project(lightricks)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DDISABLE_OPENCV_DEBUGGING)

find_package(OpenCV REQUIRED)
set(SOURCE_FILES main.cpp botox/botox.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} boost_program_options stdc++fs)
