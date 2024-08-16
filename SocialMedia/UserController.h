#pragma once
#include "User.h"

class UserController
{
public:
	void show(int id);
	void create(const std::string& userName, const std::string& email);
};

