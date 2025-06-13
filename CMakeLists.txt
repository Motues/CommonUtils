cmake_minimum_required(VERSION 3.22)

project(CommonUtils)

# 设置项目语言和标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加依赖
find_package(SQLite3 QUIET)
find_package(nlohmann_json QUIET)
find_package(Boost REQUIRED COMPONENTS system)

# 处理错误
if(SQLITE3_FOUND)
    message(STATUS "SQLite3 found")
else()
    message(STATUS "SQLite3 not found, add ThirdParty/SQLite3")
    add_subdirectory(ThirdParty/SQLite3)
endif()

if(nlohmann_json_FOUND)
    message(STATUS "nlohmann_json found")
else()
    message(STATUS "nlohmann_json not found, add ThirdParty/JSON")
    add_subdirectory(ThirdParty/JSON)
endif()

# 添加子目录
add_subdirectory(MessageQueue)
add_subdirectory(ThreadPool)
add_subdirectory(Logger)
add_subdirectory(JSON)
add_subdirectory(DataBase)
add_subdirectory(TCP)