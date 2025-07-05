#include "Utils/DataBase/SQLiteWrapper.hpp"
#include <iostream>

namespace Utils :: DataBase {
    SQLiteWrapper::SQLiteWrapper(const std::string &dbName) {
        int rc = sqlite3_open(dbName.c_str(), &db);
        if (rc != SQLITE_OK){
            std::cerr << "Can't open database: " << dbName << std::endl;
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
    }

    SQLiteWrapper::~SQLiteWrapper() {
        sqlite3_close(db);
    }

    bool SQLiteWrapper::CreateTable(const std::string& tableName, const SQLiteKeyType &columns) {
        return Model::CreateTable(db, tableName, columns);
    }

    bool SQLiteWrapper::DeleteTable(const std::string& tableName) {
        std::string sql = "DROP TABLE IF EXISTS " + tableName + ";";
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to delete table " << tableName << ": " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }

    bool SQLiteWrapper::InsertData(const std::string& tableName, const SQLiteKeyValue &data) {
        return Model::Insert(db, tableName, data);
    }

    bool SQLiteWrapper::DeleteData(const std::string& tableName, const std::string& condition) {
        return Model::Remove(db, tableName, condition);
    }

    bool SQLiteWrapper::UpdateData(const std::string& tableName, const SQLiteKeyValue &data, const std::string& condition) {
        return Model::Update(db, tableName, data, condition);
    }

    SQLiteQueryResult SQLiteWrapper::QueryData(const std::string& tableName) {
        return Model::QueryColumnAll(db, tableName);
    }

    void SQLiteWrapper::SQLiteSyntax(std::string &sql) {
        // 这里可以添加对SQL语句的检查或预处理逻辑
        // 目前仅作为占位符
    }
}