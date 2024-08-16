#include "UserController.h"
#include <iostream>

void UserController::show(int id)
{
	User user = User::find(id);
	std::cout << "User ID: " << user.getId() << std::endl;
	std::cout << "User Email: " << user.getEmail() << std::endl;
}

void UserController::create(const std::string& userName, const std::string& email)
{
	//User user;
	//user.setUserName(userName);
	//user.setEmail(email);
	//user.save();
}
