#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class ConnectionPool {
public:
    static ConnectionPool& getInstance();
    std::shared_ptr<sql::Connection> getConnection();
    void releaseConnection(std::shared_ptr<sql::Connection> connection);

    static const int MAX_POOL_CAPACITY;

private:
    ConnectionPool();
    void loadConnectionConfig(const std::string& filePath);
    std::shared_ptr<sql::Connection> createConnection();

    static ConnectionPool* instance;
    static std::mutex mtx;
    static std::condition_variable cv;
    static bool tooManyActiveConnections;
    static int reservedConnectionsCount;
    static int activeConnectionsCount;
    static int totalConnectionsCount;
    static std::queue<std::shared_ptr<sql::Connection>> pool;
    static std::string url;
    static std::string user;
    static std::string password;
};