project(ThirdParty.SQLite3)

# 设置项目语言和标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义源文件和头文件路径
file(GLOB_RECURSE SQLITE3_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SQLITE3_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 添加库目标
add_library(ThirdPartySQLite3 STATIC ${SQLITE3_HEADERS} ${SQLITE3_SOURCES})
add_library(SQLite::SQLite3 ALIAS ThirdPartySQLite3)
target_include_directories(ThirdPartySQLite3
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
