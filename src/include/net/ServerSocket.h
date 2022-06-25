#ifndef HDB_SERVER_SOCKET_H
#define HDB_SERVER_SOCKET_H
#include "Socket.h"

class ServerSocket {
   public:
    explicit ServerSocket();
    explicit ServerSocket(int port);
    /**
     * @brief 开始监听
     *
     */
    void listen();
    /**
     * @brief 处理请求
     *
     * @return Socket 客户端 Socket
     */
    Socket accept();

    /**
     * @brief close ServerScoekt
     *
     * @return int
     */
    int close();

    /**
     * @brief 从 Socket (Client) 获取消息，这个方法无法保证接受一定成功
     *
     * @return Optional<std::string>
     */
    Optional<std::string> recvice(const Socket& client);

    /**
     * @brief 像客户端发送消息
     *
     * @param client 客户端
     * @param msg 消息
     * @return int
     */
    int sentTo(const Socket& client, const std::string& message);

   private:
    sockaddr_in addr;
    int socketFd;
};

#endif  // HDB_SERVER_SOCKET_H
