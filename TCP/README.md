# CommonUtils.TCP

CommonUtils.TCP 是一个基于 Boost.Asio 的 C++ TCP 通信库，提供了 异步服务器 和 同步客户端 的封装。以下是主要类和方法的使用说明。


## 🔧 依赖项

- [Boost.Asio](https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio.html)：C++ 网络编程库。
- CommonUtils.MessageQueue

---

## 🚀 使用示例

### Client

####  1. 创建客户端对象
```cpp
using namespace Utils::TCP;
TCPClient client("127.0.0.1", 8080);
client.ConnectToServer();
```

#### 2. 发送数据
```cpp
client.SendData("Hello, Server!");
```

####  3. 接收数据
```cpp
std::string message;
client.ReceiveData(message);
```

#### 4. 断开连接
```cpp
client.CloseConnection();
```

### Server
#### 1. 创建服务器对象
```cpp
using namespace Utils::TCP;
TCPServer server(8080, "0.0.0.0");
server.StartServer();
```

#### 2. 获取客户端发来的数据
```cpp
TCPMessage message;
server.GetMessage(message);
```
客户端收到的数据以`TCPMessage`结构体形式返回，包含时间、客户端地址和数据。结构体定义如下：
```c++
struct TCPClient {
    TCPSocket socket;
    std::string address;
    int port{0};
};
struct TCPMessage {
    TimePoint time;
    TCPClientPtr clientPtr;
    std::string data;
};
```
详细描述请查看[BasicType.hpp](./include/Utils/TCP/BasicType.hpp)文件

#### 3. 发送数据给客户端
```cpp
server.SendData("Hello, Client!", clientPtr);
```

#### 4. 停止服务器
```cpp
server.CloseServer();
```


## 💡 注意事项
* 请确保 Boost.Asio 库已正确安装。
* 在实际使用中，请务必处理异常和错误。
* AsyncTCPServer 内部使用了异步 I/O 和独立线程运行，但其本身不是线程安全的，操作时需注意并发访问。

欢迎通过Issue提交建议！
> Made by Motues with ❤️
