#include "Utils/DataBase.hpp"
#include <iostream>

int main() {
    using namespace Utils::DataBase;

    // 创建数据库连接
    std::string dbFileName = "test.db";
    SQLiteWrapper dbWrapper(dbFileName);

    // 定义表结构
    SQLiteKeyType columns;
    columns["id"] = INTEGER;
    columns["name"] = TEXT;
    columns["age"] = INTEGER;

    // 创建表
    if (!dbWrapper.CreateTable("users", columns)) {
        std::cerr << "Failed to create table!" << std::endl;
        return -1;
    }

    // 插入数据
    SQLiteKeyValue data;
    data["name"] = "John Doe";
    data["age"] = "30";
    data["id"] = "1";
    if (!dbWrapper.InsertData("users", data)) {
        std::cerr << "Failed to insert data!" << std::endl;
        return -1;
    }

    // 查询所有数据
    auto result = dbWrapper.QueryData("users");
    std::cout << "Query all data:" << std::endl;
    for (const auto& row : result) {
        for (const auto& [key, value] : row) {
            std::cout << key << ": " << value << ", ";
        }
        std::cout << std::endl;
    }

    // 更新数据
    SQLiteKeyValue newData;
    newData["name"] = "Tom";
    newData["age"] = "31";
    std::string condition = "id=1";
    if (!dbWrapper.UpdateData("users", newData, condition)) {
        std::cerr << "Failed to update data!" << std::endl;
        return -1;
    }

    // 再次查询数据
    result = dbWrapper.QueryData("users");
    std::cout << "Query data after update:" << std::endl;
    for (const auto& row : result) {
        for (const auto& [key, value] : row) {
            std::cout << key << ": " << value << ", ";
        }
        std::cout << std::endl;
    }

    // 删除数据
    if (!dbWrapper.DeleteData("users", condition)) {
        std::cerr << "Failed to delete data!" << std::endl;
        return -1;
    }

    // 最后查询数据
    result = dbWrapper.QueryData("users");
    std::cout << "Query data after delete:" << std::endl;
    for (const auto& row : result) {
        for (const auto& [key, value] : row) {
            std::cout << key << ": " << value << ", ";
        }
        std::cout << std::endl;
    }

    if(!dbWrapper.DeleteTable("users")) {
        std::cerr << "Failed to delete table!" << std::endl;
        return -1;
    }


    return 0;
}