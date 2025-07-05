#pragma once

#include "BasicType.hpp"

#include <sqlite3.h>

namespace Utils :: DataBase {

    class Model {
    public:
        // 创建表
        static bool CreateTable(sqlite3 *db, const std::string& tableName, const SQLiteKeyType & columns);
        // 插入数据
        static bool Insert(sqlite3 *db, const std::string &tableName, const SQLiteKeyValue &data);
        // 查询数据
        static SQLiteQueryResult ExecuteQuery(sqlite3 *db, const std::string& sql);
        static SQLiteQueryResult QueryColumnAll(sqlite3 *db, const std::string& tableName);
        static SQLiteQueryResult QueryColumnAllCondition(sqlite3 *db, const std::string& tableName, const std::string& condition);
        static SQLiteQueryResult QueryColumn(sqlite3 *db, const std::string& tableName, std::vector<std::string> &columns);
        static SQLiteQueryResult QueryColumnCondition(sqlite3 *db, const std::string& tableName, std::vector<std::string> &columns, const std::string& condition);
        // 更新数据
        static bool Update(sqlite3 *db, const std::string& tableName, const SQLiteKeyValue &data, const std::string& condition);
        // 删除数据
        static bool Remove(sqlite3 *db, const std::string& tableName, const std::string& condition);
    };

}