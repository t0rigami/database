#ifndef HDB_NET_H
#define HDB_NET_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

namespace Net {

namespace C {

/* Close the file descriptor FD.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int _close(int fd);

/* Await a connection on socket FD.
   When a connection arrives, open a new socket to communicate with it,
   set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
   peer and *ADDR_LEN to the address's actual length, and return the
   new socket's descriptor, or -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int _accept(int fd, sockaddr* addr, socklen_t* addr_len);

/* Prepare to accept connections on socket FD.
   N connection requests will be queued before further requests are refused.
   Returns 0 on success, -1 for errors.  */
int _listen(int fd, int n);

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int _connect(int fd, sockaddr* addr, socklen_t addr_len);

/* Send N bytes of BUF to socket FD.  Returns the number sent or -1.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int _send(int fd, const void* buf, size_t size, int flags);

}  // namespace C

/**
 * @brief 默认的 Buffer 大小
 *
 */
const size_t DEFAULT_BUFFER_SIZE = 2048;
/**
 * @brief 默认的 ServerSocket 队列等待长度
 *
 */
const size_t DEFAULT_SERVER_SOCKET_QUEUE_SIZE = 10;

/**
 * @brief 创建一个 Socket
 *
 * @return scoketFd
 */
int createSocket();

/**
 * @brief 设置 SockaddrIn 结构体的值
 *
 * @param addr 目标值
 * @param ip ip
 * @param port 端口号
 */
void setSockaddrIn(sockaddr_in& addr, const char* ip, int port);

}  // namespace Net

#endif