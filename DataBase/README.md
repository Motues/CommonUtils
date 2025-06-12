# CommonUtils.DataBase

本库是对SQLite3的封装，避免繁琐的SQL语句操作，提供简单易用的接口。

## 功能概述

- 定义一个基类 `Model`，用于表示数据库中的表。
- 提供增删改查（CRUD）的通用接口。
- 自动根据类的定义生成对应的SQL语句。

## 使用说明

### 1. 初始化数据库连接

使用 `SQLiteWrapper` 类创建数据库连接，并指定日志输出方式（控制台或文件）。

```c++
using namespace Utils::DataBase;

std::string dbFileName = "test.db";
SQLiteWrapper dbWrapper(dbFileName, true, "test.log");
```

### 2. 创建表
通过 `SQLiteWrapper` 的 `CreateTable` 方法创建表。需要提供表名和字段类型映射。
```c++
// 定义表结构
SQLiteKeyType columns;
columns["id"] = INTEGER;
columns["name"] = TEXT;
columns["age"] = INTEGER;

// 创建表
if (!dbWrapper.CreateTable("users", columns)) {
    std::cerr << "Failed to create table!" << std::endl;
}
```

### 3. 插入数据
使用 `InsertData` 方法插入数据。需要提供表名和键值对数据。
```c++
// 插入数据
SQLiteKeyValue data;
data["name"] = "John Doe";
data["age"] = "30";
data["id"] = "1";
if (!dbWrapper.InsertData("users", data)) {
    std::cerr << "Failed to insert data!" << std::endl;
}
```

### 4. 查询数据
使用 `QueryData` 方法查询数据。可以查询所有数据或指定条件的数据。
```c++
// 查询所有数据
auto result = dbWrapper.QueryData("users");
std::cout << "Query all data:" << std::endl;
for (const auto& row : result) {
    for (const auto& [key, value] : row) {
        std::cout << key << ": " << value << ", ";
    }
    std::cout << std::endl;
}
```

### 5. 更新数据
使用 `UpdateData` 方法更新数据。需要提供表名、更新数据和条件。
```c++
// 更新数据
SQLiteKeyValue newData;
newData["name"] = "Tom";
newData["age"] = "31";
std::string condition = "id=1";
if (!dbWrapper.UpdateData("users", newData, condition)) {
    std::cerr << "Failed to update data!" << std::endl;
}
```

### 6. 删除数据
使用 `DeleteData` 方法删除数据。需要提供表名和删除条件
```c++
// 删除数据
if (!dbWrapper.DeleteData("users", condition)) {
    std::cerr << "Failed to delete data!" << std::endl;
}
```

### 7. 删除表
使用 `DropTable` 方法删除表。需要提供表名。
```c++
// 删除表
if (!dbWrapper.DeleteTable("users")) {
    std::cerr << "Failed to delete table!" << std::endl;
}
```

## 注意事项
* 确保已安装 `SQLite3` 库
* 日志文件路径应确保可写。
* 数据库文件路径应确保可访问。

欢迎通过Issue提交建议！
> Made by Motues with ❤️