cmake_minimum_required(VERSION 3.25)
project(ChatServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(chatServer server.cpp)

find_package(Threads REQUIRED)
target_link_libraries(chatServer PRIVATE Threads::Threads)

