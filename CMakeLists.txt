cmake_minimum_required(VERSION 3.28)

project(CommonUtils)

# 设置项目语言和标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加子目录
add_subdirectory(MessageQueue)
add_subdirectory(ThreadPool)
add_subdirectory(Logger)
add_subdirectory(DataBase)
add_subdirectory(TCP)