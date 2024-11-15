cmake_minimum_required(VERSION 3.0)
project(tp1 C)

set(CMAKE_C_STANDARD 99)

add_executable(tp1 main.c)

# add pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(tp1 Threads::Threads)

# add libc
target_link_libraries(tp1 c)

