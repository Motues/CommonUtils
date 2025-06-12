# `JSON` 模块文档

`JSON` 是一个基于 `nlohmann/json` 的轻量级 C++ 封装模块，用于简化 JSON 参数的读取、修改和持久化操作。它提供了链式访问、类型安全获取、数组索引访问等特性，适用于配置文件解析、参数管理等场景。

---

## 📦 功能特性

- ✅ **链式访问**：支持多级嵌套访问，如 `json["a"]["b"][0]["c"]`。
- ✅ **类型安全获取**：通过 `.get<T>()` 获取指定类型的值，避免类型转换错误。
- ✅ **修改值**：通过 `.set(value)` 修改任意节点的值。
- ✅ **数组索引访问**：支持通过 `json["list"][0]` 访问数组元素。
- ✅ **持久化保存**：提供 `saveToFile` 方法，将整个 JSON 树保存为文件。

---

## 🔧 依赖项

- [nlohmann/json](https://github.com/nlohmann/json)：现代 C++ JSON 库。

---

## 🚀 使用示例

### 1. 从文件加载 JSON

```cpp
JSON::JsonManager config("config.json");
```

### 2. 获取嵌套值

```cpp
int port = config["server"]["port"].get<int>();
std::string name = config["user"]["name"].String();
```
该模块提供了基本类型的转换方法，可以直接使用：

```cpp
int Int() const;
float Float() const;
double Double() const;
bool Bool() const;
std::string String() const;
```

### 3. 修改值

```cpp
config["server"]["port"].set(8080);
config["settings"]["debug"].set(true);
config["users"][0]["name"].set("Alice");
```

### 4. 保存修改后的 JSON

```cpp
config.saveToFile("updated_config.json");
```


### 5. 判断键是否存在

```cpp
bool exists(const std::string& key) const;
```

示例：

```cpp
if (config.exists("port")) {
    int port = config["port"].Int();
}
```

---


## 💡 注意事项

- **所有权管理**：根 `JsonManager` 实例拥有完整的 JSON 数据，子 `JsonManager` 实例仅引用其父节点的子结构。
- **不可拷贝**：`JsonManager` 禁用了拷贝构造函数和赋值操作符。
- **线程安全**：`JsonManager` 本身不是线程安全的，若需并发访问，需自行加锁保护。

---

欢迎通过Issue提交建议！
> Made by Motues with ❤️