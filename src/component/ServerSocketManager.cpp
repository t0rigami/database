#include "ServerSocketManager.h"

ServerSocketManager::ServerSocketManager()
        : currentProcessId(getpid()),
          currentProcessName("Main"),
          configRef(nullptr) {}

ServerSocketManager::ServerSocketManager(const DatabaseConfig &config) {
    configRef = &config;

    serverSocket = ServerSocket(config.getPort());

    setCurrentProcessInfo(getpid(), config.getServerSocketManagerName());
}

int ServerSocketManager::start() {
    serverSocket.listen();

    while (true) {
        try {
            Socket client = serverSocket.accept();
            currentProcessName = configRef->getServerSocketManagerName();

            std::cout << "Main[" << currentProcessId << "] > "
                      << "accept a client[" << client.getSocketFd() << "] .."
                      << std::endl;

            forkSubProcess(client);

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void ServerSocketManager::setCurrentProcessInfo(const pid_t &pid,
                                                const std::string &name) {
    currentProcessId = pid;
    currentProcessName = name;
}

void ServerSocketManager::forkSubProcess(const Socket &clientSocket) {
    pid_t subProcessPid = fork();

    // 创建失败
    if (subProcessPid < 0)
        throw std::runtime_error("fork sub process failed");
    else if (subProcessPid == 0) {
        // 设置子进程信息
        setCurrentProcessInfo(getpid(), configRef->getServerSocketSubName());

        // 子进程
        while (true) {
            Optional<std::string> message = serverSocket.recvice(clientSocket);

            if (message.empty()) {
                // 获取失败
                std::cout << currentProcessName + "[" << currentProcessId
                          << "] > "
                          << "client[fd=" << clientSocket.getSocketFd()
                          << "] recvice failed" << std::endl;
                break;
            } else {
                // 获取成功
                std::cout << currentProcessName + "[" << currentProcessId
                          << "] > "
                          << "client[fd=" << clientSocket.getSocketFd()
                          << "] send: " << message.get() << std::endl;

                if (message.get() == "exit") {
                    clientSocket.close();
                    // 子进程退出
                    std::cout << currentProcessName + "[" << currentProcessId
                              << "] > "
                              << "exit .." << std::endl;

                    // 父进程信息删除
                    exit(0);
                }
            }
        }
    } else {
        // 父进程
        currentProcessName = configRef->getServerSocketManagerName();

        std::cout << "Main[" << currentProcessId << "] > "
                  << "fork success, pid = " << subProcessPid << std::endl;

        backendPidList.insert(subProcessPid);
    }
}