#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"


namespace Utils :: JSON {


    class JsonManager {
    public:
        // 从文件加载 JSON
        explicit JsonManager(nlohmann::json& json) {
            json_ = json;
            ptr_ = &json_;
        }

        static JsonManager FromFile(const std::string& filePath) {
            std::ifstream in(filePath);
            nlohmann::json json{};
            if (!in.is_open()) {
                std::cout << "无法打开文件：" << filePath << std::endl;
                return JsonManager{json};
            }
            try {
                in >> json;
            } catch (const nlohmann::json::parse_error& e) {
                std::cout << "JSON解析错误: " << e.what() << std::endl;
            }
            return JsonManager(json);
        }

        static JsonManager FromString(const std::string& jsonString) {
            nlohmann::json json;
            try {
                json = nlohmann::json::parse(jsonString);
            } catch (const nlohmann::json::parse_error& e) {
                std::cout << "JSON解析错误: " << e.what() << std::endl;
            }
            return JsonManager(json);
        }

        std::string toString(int indent = 0) const {
            if(indent == 0) return json_.dump();
            return json_.dump(indent);
        }

        // 从现有 JSON 对象构造（用于嵌套访问）
        explicit JsonManager(nlohmann::json* ptr) : ptr_(ptr), isOwner_(false) {}

        // 禁止拷贝构造和赋值
        JsonManager(const JsonManager&) = delete;
        JsonManager& operator=(const JsonManager&) = delete;

        // 通过字符串键访问子节点
        JsonManager operator[](const std::string& key) const {
            if (!ptr_->contains(key)) {
                std::cout << "参数不存在：" << key << std::endl;
            }
            return JsonManager(&(*ptr_)[key]);
        }

        // 通过整数索引访问数组元素
        JsonManager operator[](size_t index) const {
            if (!ptr_->is_array() || index >= ptr_->size()) {
                std::cout << "数组索引越界或非数组类型" << std::endl;
            }
            return JsonManager(&(*ptr_)[index]);
        }

        //
        JsonManager operator[](const std::string& key) {
            if (!ptr_->is_object()) {
                *ptr_ = nlohmann::json::object(); // 如果不是对象则初始化为空对象
            }
            return JsonManager(&(*ptr_)[key]);   // 自动创建键
        }

        JsonManager operator[](size_t index) {
            if (!ptr_->is_array()) {
                *ptr_ = nlohmann::json::array(); // 如果不是数组则初始化为空数组
            }
            while (index >= ptr_->size()) {
                ptr_->push_back(nullptr);       // 自动填充空元素到目标索引
            }
            return JsonManager(&(*ptr_)[index]);
        }

        template<typename T>
        JsonManager& operator=(const T& value) {
            *ptr_ = value;
            return *this;
        }

        // 获取值
        template<typename T>
        [[nodiscard]] T get() const {
            return ptr_->get<T>();
        }

        // 允许隐式转换
        template<typename T>
        explicit operator T() const {
            return ptr_->get<T>();
        }

        // 设置值
        template<typename T>
        void set(const T& value) {
            *ptr_ = value;
        }

        // 快捷类型转换方法
        [[nodiscard]] int Int() const { return get<int>(); }
        [[nodiscard]] float Float() const { return get<float>(); }
        [[nodiscard]] double Double() const { return get<double>(); }
        [[nodiscard]] bool Bool() const { return get<bool>(); }
        [[nodiscard]] std::string String() const { return get<std::string>(); }

        // 检查当前节点是否存在
        bool exists(const std::string& key) const {
            return ptr_->contains(key);
        }

        // 保存整个 JSON 到文件
        void saveToFile(const std::string& filePath) const {
            std::ofstream out(filePath);
            if (!out.is_open()) {
                return;
            }
            out << json_.dump(4);  // 格式化缩进 4 字符
        }

    private:
        nlohmann::json json_;          // 根对象的数据
        nlohmann::json* ptr_;          // 指向当前节点
        bool isOwner_ = true;         // 是否拥有 json_ 的生命周期
    };

}