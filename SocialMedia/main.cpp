#include <iostream>
#include <thread>
#include <vector>
#include "ConnectionPool.h"
#include "MySQLUserDAO.h"

static int counter = 0;

void getUserByEmailInParallel(const std::string& email) {
    MySQLUserDAO userDAO;

    try {
        userDAO.getUserByEmail(email);
        std::cout << ++counter << " User email(s) retrieved." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        const int numThreads = 20;
        std::vector<std::thread> threads;
        std::string email = "howdy";

        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back(getUserByEmailInParallel, email);
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}