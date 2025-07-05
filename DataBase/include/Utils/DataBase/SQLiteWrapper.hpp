#pragma once

#include "Model.hpp"
#include "BasicType.hpp"

namespace Utils :: DataBase {
    class SQLiteWrapper {
    public:
        SQLiteWrapper(const std::string &dbFileName);
        ~SQLiteWrapper();

        bool CreateTable(const std::string& tableName, const SQLiteKeyType &columns);
        bool DeleteTable(const std::string& tableName);
        bool InsertData(const std::string& tableName, const SQLiteKeyValue &data);
        bool DeleteData(const std::string& tableName, const std::string& condition);
        bool UpdateData(const std::string& tableName, const SQLiteKeyValue &data, const std::string& condition);
        SQLiteQueryResult QueryData(const std::string &tableName);
        void SQLiteSyntax(std::string &sql);

        sqlite3* GetDatabase() const { return db; }

    private:
        sqlite3* db;
    };
}