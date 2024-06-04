cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)

project(coding-theory LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/haffman.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/shannon_fano.cpp
)

add_executable(demo main.cpp)

#target_include_directories(${PROJECT_NAME} PUBLIC
#    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
#    "$<INSTALL_INTERFACE:include>"
#)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(demo ${PROJECT_NAME})
