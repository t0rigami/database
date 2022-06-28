#ifndef HDB_DATABASE_CONFIG_H
#define HDB_DATABASE_CONFIG_H

#include "Properties.h"

class DatabaseConfig {
public:
    DatabaseConfig();

    /**
     * 从 properties 中加载配置
     * @param config properties
     */
    explicit DatabaseConfig(Properties &config);

    /**
     * 从文件中加载配置
     * @param path 配置文件路径
     */
    explicit DatabaseConfig(const std::string &path);

    /**
     * 获取 port
     * @return
     */
    const int &getPort() const;

    /**
     * 日志打印名称
     * @return
     */
    const std::string &getServerSocketManagerName() const;

    /**
     * 日志打印名称
     * @return
     */
    const std::string &getServerSocketSubName() const;

    /**
     * 表的存储路径
     * @return
     */
    const std::string &getTablePath() const;

    /**
     * 表中的页存储路径
     * @return
     */
    const std::string &getPagePath() const;

    /**
     * 临时数据存放的路径
     * @return 临时文件
     */
    const std::string &getTempPath() const;

private:
    void setConfig(Properties &config);

    int _port{8888};
    std::string _serverSocketManagerName{"Main"};
    std::string _serverSocketSubName{"Sub"};
    std::string _tablePath{"./table"};
    std::string _pagePath{"./page"};
    std::string _tempPath{"./tmp"};
};

#endif  // HDB_DATABASE_CONFIG_H
