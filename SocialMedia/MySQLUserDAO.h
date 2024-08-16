#pragma once
#include <string>
#include "IUserDAO.h"

class MySQLUserDAO : public IUserDAO
{
public:
	User getUserByEmail(const std::string& email) override;
	bool comparePassword(const std::string& password) override;
private:
	static const std::string QUERY_GET_USER_BY_EMAIL;
};