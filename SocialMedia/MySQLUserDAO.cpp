#include "MySQLUserDAO.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <memory>
#include <random>
#include "ConnectionPool.h"
#include <chrono>
#include <thread>

const std::string MySQLUserDAO::QUERY_GET_USER_BY_EMAIL =
"SELECT * FROM combo.users WHERE email = ?";

User MySQLUserDAO::getUserByEmail(const std::string& email) {
    User user("");
    ConnectionPool& pool = ConnectionPool::getInstance();
    std::shared_ptr<sql::Connection> con;

    try {
        con = pool.getConnection();
        if (!con) {
            std::cerr << "Failed to obtain database connection." << std::endl;
            return user;
        }

        std::cout << "Connection obtained successfully." << std::endl;

        try {
            con->setSchema("combo");
            std::cout << "Schema 'combo' set successfully." << std::endl;
        }
        catch (const sql::SQLException& e) {
            std::cerr << "SQL Exception while setting schema: " << e.what() << std::endl;
            throw;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception while setting schema: " << e.what() << std::endl;
            throw;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(MySQLUserDAO::QUERY_GET_USER_BY_EMAIL));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        // Random connection idle times for test purposes
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, 5);

        std::this_thread::sleep_for(std::chrono::seconds(dist(rng)));
        if (res->next()) {
            user.setId(res->getInt("id"));
            user.setEmail(res->getString("email"));
        }

        pool.releaseConnection(con);
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }

    return user;
}


bool MySQLUserDAO::comparePassword(const std::string& password)
{
	// TODO: implement MySQL compare password
	return false;
}
