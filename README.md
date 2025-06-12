# CommonUtils

CommonUtils 是一个通用工具库，旨在简化C++开发中的常见任务。它包含了线程池、消息队列、日志记录、数据库操作和TCP通信等功能模块。

## 快速开始

```bash
git clone https://github.com/Motues/CommonUtils.git
cd CommonUtils
mkdir build && cd build
cmake ..
make -j
```

## 模块介绍

### MessageQueue
- **功能**: 提供线程安全的消息队列功能。
- **依赖**: 无。

### ThreadPool
- **功能**: 提供线程安全的程池功能。
- **依赖**: 无。

### Logger
- **功能**: 提供线程安全的日志记录功能。
- **依赖**: 无。

### JSON
- **功能**: 提供JSON数据处理功能。
- **依赖**: 
  - [nlohmann/json](https://github.com/nlohmann/json)

### Database
- **功能**: 对SQLite3数据库进行封装，简化操作。
- **依赖**: 
  - [SQLite3](https://www.sqlite.org/index.html)
  - Utils::Logger

### TCP
- **功能**: 提供TCP网络通信功能。
- **依赖**: 
  - Boost.Asio
  - Utils::MessageQueue

请确保在使用相关模块时，正确安装和配置其依赖项。

> Made by Motues with ❤️