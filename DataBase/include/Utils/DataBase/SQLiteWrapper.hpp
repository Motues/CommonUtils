#pragma once

#include "Utils/Logger.hpp"
#include "Model.hpp"
#include "BasicType.hpp"

#include <sqlite3.h>


namespace Utils :: DataBase {



	class SQLiteWrapper {
	public:
        SQLiteWrapper(std::string &dbFileName, bool consoleLogger = true, const std::string &logFile = "DataBase.log");
		~SQLiteWrapper();

        void CreateTable();
        void DeleteTable();
        void InsertData();
        void DeleteData();
        void UpdateData();
        SQLiteQueryResult QueryData(const std::string &sql);
        void SQLitesyntax(std::string &sql);

	private:
		sqlite3* db;
        Logger::Logger logger;

        void ConfigureLogger(bool consoleLogger = true, const std::string &logFile = "DataBase.log");
	};
}
