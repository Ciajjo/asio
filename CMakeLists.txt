cmake_minimum_required(VERSION 3.15)
project(asio)


# 设置编译模式为 Debug 用gdb调试
# set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#寻找头文件和库的目录
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB MAINSRC ${PROJECT_SOURCE_DIR}/*.cpp)
# aux_source_directory

add_library(hjk SHARED ${SRC})

#生成可执行程序
add_executable(app ${MAINSRC})

#连接动态库
target_link_libraries(app hjk)