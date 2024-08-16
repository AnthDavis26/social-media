#include "User.h"

#pragma warning(disable : 26495)
User::User(const std::string& email) { this->email = email; }

int User::getId() const { return this->id; }

std::string User::getEmail() const { return this->email; }

void User::setId(int id) { this->id = id; }

void User::setEmail(const std::string& email) { this->email = email; }
