#include "Net.h"
#include <cstring>
#include <iostream>
#include "Assert.h"

int Net::createSocket() {
    // 创建 socket
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    Assert::isFalse(socketFd == -1, "create socket failed");

    return socketFd;
}

void Net::setSockaddrIn(sockaddr_in& addr, const char* ip, int port) {
    // 协议族 IPv4
    addr.sin_family = AF_INET;
    // 端口
    addr.sin_port = htons(port);
    // ip 地址
    addr.sin_addr.s_addr = inet_addr(ip);
    // 填充 0
    memset(&(addr.sin_zero), 0, sizeof(addr.sin_zero));
}

int Net::C::_close(int fd) {
    return close(fd);
}

int Net::C::_accept(int fd, sockaddr* addr, socklen_t* addr_len) {
    return accept(fd, addr, addr_len);
}

int Net::C::_listen(int fd, int n) {
    return listen(fd, n);
}

int Net::C::_connect(int fd, sockaddr* addr, socklen_t addr_len) {
    return connect(fd, addr, addr_len);
}

int Net::C::_send(int fd, const void* buf, size_t size, int flags) {
    return send(fd, buf, size, flags);
}