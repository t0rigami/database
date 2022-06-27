#ifndef HDB_SERVER_SOCKET_MANAGER_H
#define HDB_SERVER_SOCKET_MANAGER_H

#include <queue>
#include <unordered_set>
#include "DatabaseConfig.h"
#include "ServerSocket.h"

/**
 * @brief DB 的 socket 通信管理
 *
 */
class ServerSocketManager {
public:
    explicit ServerSocketManager();

    explicit ServerSocketManager(const DatabaseConfig &config);

    /**
     * @brief 开始服务
     *
     * @return int
     */
    int start();

private:
    /**
     * @brief fork 一个子进程服务
     *
     * @return 子进程 pid_t
     */
    void forkSubProcess(const Socket &clientSocket);

    /**
     * @brief 设置当前进程的一些信息
     *
     * @param pid pid
     * @param name 标识符
     */
    inline void setCurrentProcessInfo(const pid_t &pid,
                                      const std::string &name);

    DatabaseConfig const *configRef;

    ServerSocket serverSocket;
    std::unordered_set<pid_t> backendPidList;

    /**
     * @brief 当前进程的 Pid
     *
     */
    pid_t currentProcessId{};
    /**
     * @brief 当前进程的标识符
     *
     */
    std::string currentProcessName;
};

#endif  // HDB_SERVER_SOCKET_MANAGER_H
