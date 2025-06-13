project(Utils.JSON)

# 设置项目语言和标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 定义源文件和头文件路径
file(GLOB_RECURSE JSON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
# file(GLOB_RECURSE JSON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")


# 添加库目标
add_library(UtilsJSON INTERFACE ${JSON_HEADERS} ${JSON_SOURCES})
add_library(Utils::JSON ALIAS UtilsJSON)
target_include_directories(UtilsJSON
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(UtilsJSON
    INTERFACE nlohmann_json::nlohmann_json
)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${test_source} NAME_WE)
    set(EXECUTABLE_NAME "JSON_${TEST_NAME}")
    add_executable(${EXECUTABLE_NAME} ${test_source})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Utils::JSON)
endforeach()