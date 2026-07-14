cmake_minimum_required(VERSION 3.10)
project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

# 声明可执行文件及其对应源码
add_executable(thread_01 src/thread_01.cpp)
add_executable(thread_02 src/thread_02.cpp)
add_executable(mutex_lock_01 src/mutex_lock_01.cpp)
add_executable(mutex_lock_02 src/mutex_lock_02.cpp)
add_executable(deadlock_01 src/deadlock_01.cpp)
add_executable(atomic_01 src/atomic_01.cpp)

add_executable(thread_function src/thread_function.cpp)
add_executable(thread_join src/thread_join.cpp)

add_executable(condition_variable_01 src/condition_variable_01.cpp)
add_executable(condition_variable_02 src/condition_variable_02.cpp)

add_executable(semaphore_01 src/semaphore_01.cpp)

