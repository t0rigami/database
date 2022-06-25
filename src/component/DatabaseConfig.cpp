#include "DatabaseConfig.h"
#include "StringUtils.h"

DatabaseConfig::DatabaseConfig() = default;

DatabaseConfig::DatabaseConfig(Properties &config) {
    this->setConfig(config);
}

const int &DatabaseConfig::getPort() const {
    return _port;
}

DatabaseConfig::DatabaseConfig(const std::string &path) {
    Properties config(path);
    this->setConfig(config);
}

void DatabaseConfig::setConfig(Properties &properties) {
    std::string port = properties.get("port");
    if (StringUtils::isNotBlank(port))
        this->_port = std::stoi(port);

    const std::string &serverSocketManagerName =
            properties.get("server.socket.manager.name");
    if (StringUtils::isNotBlank(serverSocketManagerName)) {
        this->_serverSocketManagerName = serverSocketManagerName;
    }

    const std::string &serverSocketSubName = properties.get("server.socket.sub.name");
    if (StringUtils::isNotBlank(serverSocketSubName)) {
        this->_serverSocketSubName = serverSocketSubName;
    }

    const std::string &tablePath = properties.get("table.path");
    if (StringUtils::isNotBlank(tablePath)) {
        this->_tablePath = tablePath;
    }

    const std::string &pagePath = properties.get("page.path");
    if (StringUtils::isNotBlank(pagePath)) {
        this->_pagePath = pagePath;
    }
}

const std::string &DatabaseConfig::getServerSocketManagerName() const {
    return _serverSocketManagerName;
}

const std::string &DatabaseConfig::getServerSocketSubName() const {
    return _serverSocketSubName;
}

const std::string &DatabaseConfig::getTablePath() const {
    return _tablePath;
}

const std::string &DatabaseConfig::getPagePath() const {
    return _pagePath;
}
