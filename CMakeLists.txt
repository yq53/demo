# 要求cmake最低是多少版本
cmake_minimum_required(VERSION 3.10)

# 声明工程名称
project(demo)

# 设置可执行文件输出位置
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 声明可执行文件及其对应源码
add_executable(thread_01 src/thread_01.cpp)
add_executable(thread_02 src/thread_02.cpp)
add_executable(mutex_lock_01 src/mutex_lock_01.cpp)
add_executable(mutex_lock_02 src/mutex_lock_02.cpp)
add_executable(deadlock_01 src/deadlock_01.cpp)
add_executable(atomic_01 src/atomic_01.cpp)


