cmake_minimum_required(VERSION 3.28)

project(ThirdParty.JSON)

# 设置项目语言和标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义源文件和头文件路径
file(GLOB_RECURSE NLOHMANN_JSON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
#file(GLOB_RECURSE NLOHMANN_JSON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 添加库目标
add_library(ThirdPartyJSON INTERFACE ${NLOHMANN_JSON_HEADERS})
add_library(nlohmann_json::nlohmann_json ALIAS ThirdPartyJSON)
target_include_directories(ThirdPartyJSON
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
