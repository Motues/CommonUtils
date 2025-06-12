#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"


namespace Utils :: JSON {


    class JsonManager {
    public:
        // 从文件加载 JSON
        explicit JsonManager(const std::string& filePath) {
            std::ifstream in(filePath);
            if (!in.is_open()) {
                std::cout << "无法打开文件：" << filePath << std::endl;
                return;
            }
            try {
                in >> json_;
            } catch (const nlohmann::json::parse_error& e) {
                std::cout << "JSON解析错误: " << e.what() << std::endl;
            }
            ptr_ = &json_;
        }

        // 从现有 JSON 对象构造（用于嵌套访问）
        explicit JsonManager(nlohmann::json* ptr) : ptr_(ptr), isOwner_(false) {}

        // 禁止拷贝构造和赋值
        JsonManager(const JsonManager&) = delete;
        JsonManager& operator=(const JsonManager&) = delete;

        // 通过字符串键访问子节点
        JsonManager operator[](const std::string& key) {
            if (!ptr_->contains(key)) {
                std::cout << "参数不存在：" << key << std::endl;
            }
            return JsonManager(&(*ptr_)[key]);
        }

        // 通过整数索引访问数组元素
        JsonManager operator[](size_t index) {
            if (!ptr_->is_array() || index >= ptr_->size()) {
                std::cout << "数组索引越界或非数组类型" << std::endl;
            }
            return JsonManager(&(*ptr_)[index]);
        }

        // 获取值
        template<typename T>
        [[nodiscard]] T get() const {
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