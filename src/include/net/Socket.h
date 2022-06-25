#ifndef HDB_SOCKET_H
#define HDB_SOCKET_H
#include <iostream>
#include "Net.h"
#include "Optional.h"

class Socket {
   public:
    explicit Socket(int socketFd, sockaddr_in addr);
    explicit Socket(std::string serverHost, int serverPort);

    /**
     * @brief 连接 ServerScoket
     *
     * @return int -1 失败
     */
    int connect();

    /**
     * @brief 发送消息
     *
     * @param message 消息
     * @return int
     */
    int send(const std::string& message);

    /**
     * @brief 关闭 Socket
     *
     * @return int 返回 -1 则未正常关闭
     */
    int close() const;

    /**
     * @brief 从 Server 中接受消息
     *
     * @return Optional<std::string>
     */
    Optional<std::string> recviceFromServer();

    /**
     * @brief 获取 SocketFd
     *
     * @return int
     */
    int getSocketFd() const;

   private:
    hostent* serverHost;
    sockaddr_in serverAddr;
    int socketFd;
};
#endif