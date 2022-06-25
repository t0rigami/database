#include "ServerSocket.h"
#include "Assert.h"

ServerSocket::ServerSocket() = default;

ServerSocket::ServerSocket(int port) {
    // 创建 socket
    socketFd = Net::createSocket();

    Net::setSockaddrIn(this->addr, "127.0.0.1", port);

    // bind
    int bs = bind(socketFd, (sockaddr*)&addr, sizeof(sockaddr));

    Assert::isFalse(bs == -1, "failed cause bind addr");
}

void ServerSocket::listen() {
    int ls = Net::C::_listen(socketFd, Net::DEFAULT_SERVER_SOCKET_QUEUE_SIZE);
    Assert::isFalse(ls == -1, "failed cause listen addr");
}

Socket ServerSocket::accept() {
    sockaddr_in socketClient;
    socklen_t sinSize;

    int clientFd =
        Net::C::_accept(socketFd, (sockaddr*)&socketClient, &sinSize);

    Assert::isFalse(clientFd == -1, "failed to accept client");

    return Socket(clientFd, socketClient);
}

Optional<std::string> ServerSocket::recvice(const Socket& client) {
    int clientSocketFd = client.getSocketFd();
    // 准备缓冲区
    char buf[Net::DEFAULT_BUFFER_SIZE] = {0};

    if (recv(clientSocketFd, buf, Net::DEFAULT_BUFFER_SIZE, 0) == -1)
        return Optional<std::string>();

    return Optional<std::string>(std::string(buf));
}

int ServerSocket::close() {
    return Net::C::_close(socketFd);
}

int ServerSocket::sentTo(const Socket& client, const std::string& message) {
    int clientSocketFd = client.getSocketFd();
    auto msg = message.c_str();
    return Net::C::_send(clientSocketFd, msg, sizeof(char) * message.size(), 0);
}