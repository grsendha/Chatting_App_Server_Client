cmake_minimum_required(VERSION 3.25)
project(ChatClient)

set(CMAKE_CXX_STANDARD 11)

add_executable(chatClient client.cpp)

find_package(Threads REQUIRED)
target_link_libraries(chatClient PRIVATE Threads::Threads)
