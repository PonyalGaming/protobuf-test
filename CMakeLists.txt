cmake_minimum_required(VERSION 3.12)
project(protobuf_test LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11")
set(CMAKE_CXX_STANDARD 11)

INCLUDE(FindProtobuf)

FIND_PACKAGE(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIR})

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER visitInfo.proto)
add_library(proto2 ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(proto2)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(visitInfo main.cpp)

target_link_libraries(visitInfo proto2 ${PROTOBUF_LIBRARY})
