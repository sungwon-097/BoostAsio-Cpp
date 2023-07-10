cmake_minimum_required(VERSION 3.25)

project(ChatServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

set(BOOST_ROOT $ENV{HOME}/sungwon/Documents/ChatServer/boost_1_82_0)
set(Boost_INCLUDE_DIR ${BOOST_ROOT})
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib)

find_package(Boost REQUIRED COMPONENTS regex REQUIRED)

# Boost 라이브러리의 헤더 파일 경로 설정
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(ChatServer webSocketServer.cpp)
target_link_libraries( ChatServer ${Boost_LIBRARIES} )