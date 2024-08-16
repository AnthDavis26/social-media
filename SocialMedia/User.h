#pragma once
#include <string>

class User
{
public:
	User(const std::string& email);

	int getId() const;
	std::string getEmail() const;

	void setId(int id);
	void setEmail(const std::string& email);

private:
	int id;
	std::string email;
};

