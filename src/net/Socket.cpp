#include "Socket.h"
#include "Assert.h"

Socket::Socket(int socketFd, sockaddr_in addr)
    : socketFd(socketFd), serverAddr(addr) {}

Socket::Socket(std::string serverHost, int serverPort) {
    this->serverHost = gethostbyname(serverHost.c_str());

    socketFd = Net::createSocket();

    Net::setSockaddrIn(this->serverAddr, serverHost.c_str(), serverPort);
}

int Socket::connect() {
    return Net::C::_connect(socketFd, (sockaddr*)&serverAddr, sizeof(sockaddr));
}

int Socket::send(const std::string& message) {
    auto msg = message.c_str();
    return Net::C::_send(socketFd, msg, message.size() * sizeof(char), 0);
}

Optional<std::string> Socket::recviceFromServer() {
    // 准备缓冲区
    char buf[Net::DEFAULT_BUFFER_SIZE] = {0};

    if (recv(socketFd, buf, Net::DEFAULT_BUFFER_SIZE, 0) == -1)
        return Optional<std::string>();

    return Optional<std::string>(std::string(buf));
}

int Socket::close() const {
    return Net::C::_close(socketFd);
}

int Socket::getSocketFd() const {
    return socketFd;
}