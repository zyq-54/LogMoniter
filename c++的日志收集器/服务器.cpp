
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<vector>
#include<winsock2.h>
#include<thread>
#include<mutex>
#include<chrono>
#include<queue>
#include <condition_variable>
#pragma comment(lib,"ws2_32.lib")
#include<memory>
using namespace std;
mutex mtx;
FILE* file = nullptr;
queue<string>data_queue;
std::condition_variable cv;
const size_t maxfilesize = 100 * 1024 * 1024;

//缓冲区写入文件
int flush_buffer(string& buffer) {
    cout << "1" << endl;
    if (!file || buffer.empty())return 0;
    lock_guard<mutex>lock(mtx);
    long cur_size = ftell(file);
    if (cur_size < 0)cur_size = 0;
    if (cur_size + buffer.size() > maxfilesize) {
        if (fclose(file) != 0) {
            cerr << "关闭文件失败" << endl;
            fwrite(buffer.data(), 1, buffer.size(), file);
            fflush(file);
            buffer.clear();
            return -1;
        }
        file = nullptr;
        remove("output_1.txt");
        if (rename("output.txt", "output_1.txt") != 0) {
            cout << "重命名文件失败" << endl;
        }
        file = fopen("output.txt", "ab");
        if (!file) {
            cerr << "轮转后打开文件失败" << endl;
            return -1;
        }
    }
    fwrite(buffer.data(), 1, buffer.size(), file);
    fflush(file);
    buffer.clear();
    return 0;
}
void producer(SOCKET client_sock) {
    int timeout = 5000; // 毫秒
    if (setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
        cerr << "设置超时失败: " << WSAGetLastError() << endl;
        closesocket(client_sock);
        return;
    }
    auto start = chrono::steady_clock::now();
    const int timesecond = 3;
    char a[4096];
    int received;
    string buffer;
    //接受客户端访问
    while (true) {
        received = recv(client_sock, a, sizeof(a), 0);
        if (received > 0) {
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
            buffer.append(a, received);
            if (buffer.size() >= 1024 * 1024 || elapsed >= timesecond) {
                string filtered;
                istringstream stream(buffer);
                string line;
                while (getline(stream, line)) {
                    if (line.find("[ERROR]") == 0) {
                        auto now = chrono::system_clock::now();
                        auto now_time = chrono::system_clock::to_time_t(now);
                        struct tm time_info;
                        localtime_s(&time_info, &now_time);
                        char time_buf[20];
                        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &time_info);
                        string timestamp(time_buf);
                        filtered += timestamp + " " + line + "\n";
                    }
                }
                if (!filtered.empty()) {
                    lock_guard<mutex> lock(mtx);
                    data_queue.push(filtered);
                    cv.notify_one();
                }
                buffer.clear();
                start = now;
            }
        }
        else if (received == 0) {
            cout << "客户端正常断开" << endl;
            break;
        }
        else {
            int err = WSAGetLastError();
            if (err == WSAETIMEDOUT) {
                cout << "客户端5秒无数据，主动断开" << endl;
                break;
            }
            else {
                cerr << "recv错误" << err << endl;
                break;
            }
        }
        if (!buffer.empty()) {
            string filtered;
            istringstream stream(buffer);
            string line;
            while (getline(stream, line)) {
                if (line.find("[ERROR]") == 0) {
                    auto now = chrono::system_clock::now();
                    auto now_time = chrono::system_clock::to_time_t(now);
                    struct tm time_info;
                    localtime_s(&time_info, &now_time);
                    char time_buf[20];
                    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &time_info);
                    string timestamp(time_buf);
                    filtered += timestamp + " " + line + "\n";
                }
            }
            if (!filtered.empty()) {
                lock_guard<mutex> lock(mtx);
                data_queue.emplace(move(filtered));
                cv.notify_one();
            }
        }
    }
    closesocket(client_sock);
}
void consumer() {
    auto start = chrono::steady_clock::now();
    string temp_buffer;
    temp_buffer.reserve(10 * 1024 * 1024);
    while (true) {
        string data; {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] {return !data_queue.empty(); });
            data = move(data_queue.front());
            data_queue.pop();
        }
        temp_buffer.insert(temp_buffer.end(), data.begin(), data.end());
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
        if (temp_buffer.size() >= 10 * 1024 * 1024 || elapsed >= 10) {
            flush_buffer(temp_buffer);
            temp_buffer.clear();
            start = now;
        }
    }
}
int main() {
    WSADATA wsadata;
    int ret1 = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (ret1 != 0) {
        cerr << "WSAStartup 失败，错误码: " << ret1 << endl;
        return 1;
    }
    file = fopen("output.txt", "ab");
    if (!file) {
        cerr << "无法打开文件" << endl;
        return -1;
    }

    vector<thread>threads;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "bind失败" << WSAGetLastError() << endl;
        return -1;
        fclose(file);
        closesocket(sock);
        WSACleanup();
    }
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "listen失败" << WSAGetLastError() << endl;
        closesocket(sock);
        fclose(file);
        WSACleanup();
        return -1;
    }
    cout << "等待连接" << endl;
    if (!file) {
        cerr << "无法打开文件" << endl;
        return -1;
    }
    thread consumer_thread(consumer);
    consumer_thread.detach();
    while (true) {
        SOCKET client_sock = accept(sock, nullptr, nullptr);
        if (client_sock == INVALID_SOCKET) {
            cerr << "accept failed" << WSAGetLastError() << endl;
            continue;
        }
        cerr << "客户端连接" << endl;
        threads.emplace_back(producer, client_sock);
    }
    closesocket(sock);
    fclose(file);
    WSACleanup();
    return 0;
}