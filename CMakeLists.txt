# 要求cmake最低是多少版本
cmake_minimum_required(VERSION 3.10)

# 声明工程名称
project(demo)

# 设置可执行文件输出位置
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 声明可执行文件及其对应源码
add_executable(demo src/demo.cpp)


