cmake_minimum_required(VERSION 3.12)

project(backup)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC main.cpp backup.cpp)

target_link_libraries(${PROJECT_NAME} libcommon)