#pragma once
#include "User.h"
#include <vector>

class IUserDAO
{
public:
    virtual ~IUserDAO() = default;
    virtual User getUserByEmail(const std::string& email) = 0;
    virtual bool comparePassword(const std::string& password) = 0;
};
