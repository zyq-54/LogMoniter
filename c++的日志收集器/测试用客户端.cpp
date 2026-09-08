#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
    // 1. 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // 2. 创建套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. 连接服务器（本机 8080 端口）
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "连接失败 " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    std::cout << "连接成功" << std::endl;

    // 4. 发送 200 条测试消息
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < 30; ++i) {
        std::string msg;
        if (dis(gen) < 0.3) {  // 30% 概率为 ERROR
            msg = "[ERROR] 测试错误消息 #" + std::to_string(i + 1) + "\n";
        }
        else {
            msg = "[INFO] 普通信息 #" + std::to_string(i + 1) + "\n";
        }
        if (send(sock, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
            std::cerr << "发送失败 " << WSAGetLastError() << std::endl;
            break;
        }
        // 每 10 条稍作停顿，模拟真实流量
    }

    std::cout << "发送完成" << std::endl;
    // 等待服务器处理完缓冲区数据
    std::this_thread::sleep_for(std::chrono::seconds(1));

    closesocket(sock);
    WSACleanup();
    std::cout << "关闭连接" << std::endl;
    return 0;
}