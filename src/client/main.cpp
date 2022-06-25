#include <iostream>
#include "Assert.h"
#include "Socket.h"
using namespace std;
#include <ctime>
#include "Optional.h"

int main(int argc, char const* argv[]) {
    // 建立一个客户端
    Socket clientSocket("127.0.0.1", 8888);
    // 尝试连接
    int t = clientSocket.connect();
    Assert::isFalse(t == -1, "client connect failed");

    string msg;
    while (true) {
        // 读取控制台程序
        cin >> msg;
        // 发送消息
        t = clientSocket.send(msg);
        Assert::isFalse(t == -1, "client send failed");
        cout << "send size: " << t << endl;
        cout << "send: " << msg << endl;
        if (msg == "exit")
            break;
    }

    clientSocket.close();
    return 0;
}
