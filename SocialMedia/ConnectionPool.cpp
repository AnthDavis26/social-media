#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::instance = nullptr;
std::mutex ConnectionPool::mtx;
std::condition_variable ConnectionPool::cv;
bool ConnectionPool::tooManyActiveConnections = false;
const int ConnectionPool::MAX_POOL_CAPACITY = 8;
int ConnectionPool::reservedConnectionsCount = 0;
int ConnectionPool::activeConnectionsCount = 0;
int ConnectionPool::totalConnectionsCount = 0;
std::queue<std::shared_ptr<sql::Connection>> ConnectionPool::pool;
std::string ConnectionPool::url;
std::string ConnectionPool::user;
std::string ConnectionPool::password;

ConnectionPool::ConnectionPool() {
    loadConnectionConfig("connection_config.json");
}

void ConnectionPool::loadConnectionConfig(const std::string& filePath) {
    std::ifstream configFile(filePath);

    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open connection config file: " + filePath);
    }

    nlohmann::json data;
    configFile >> data;
    url = data["url"];
    user = data["user"];
    password = data["password"];
}

ConnectionPool& ConnectionPool::getInstance() {
    static ConnectionPool instance;
    return instance;
}

std::shared_ptr<sql::Connection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Getting connection..." << std::endl;
    std::cout << "Active: " << activeConnectionsCount << std::endl;
    std::cout << "Reserved: " << reservedConnectionsCount << std::endl;
    std::cout << "Total: " << totalConnectionsCount << std::endl;

    while (activeConnectionsCount >= MAX_POOL_CAPACITY) {
        std::cout << "Connection pool is full. Waiting for a connection to be released..." << std::endl;
        cv.wait(lock);
    }

    activeConnectionsCount++;
    std::cout << "Connection obtained." << std::endl;

    if (reservedConnectionsCount > 0) {
        auto con = std::move(pool.front());
        pool.pop();
        reservedConnectionsCount--;
        return con;
    }

    totalConnectionsCount++;
    return createConnection();
}

void ConnectionPool::releaseConnection(std::shared_ptr<sql::Connection> connection) {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Releasing connection..." << std::endl;
    std::cout << "Active: " << activeConnectionsCount << std::endl;
    std::cout << "Reserved: " << reservedConnectionsCount << std::endl;
    std::cout << "Total: " << totalConnectionsCount << std::endl;

    pool.push(connection);
    activeConnectionsCount--;
    reservedConnectionsCount++;

    std::cout << "Connection released." << std::endl;
    cv.notify_one();
}

std::shared_ptr<sql::Connection> ConnectionPool::createConnection() {
    try {
        sql::Driver* driver = get_driver_instance();
        std::shared_ptr<sql::Connection> connection(driver->connect(url, user, password));
        return connection;
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        throw;
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating connection: " << e.what() << std::endl;
        throw;
    }
}
