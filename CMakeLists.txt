cmake_minimum_required(VERSION 3.28.3)

project(atmosphere)

add_executable(${PROJECT_NAME} atmosphere.cpp)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
