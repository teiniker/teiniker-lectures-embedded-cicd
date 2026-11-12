#pragma once

#include <string>
#include <vector>
#include "user.h"

class UserTable 
{
	private:
		std::vector<User> _table; 
 
	public:
		UserTable(void);
    
		void insertUser(const User& user);
		void updateUser(const User& user);
		User& findById(const int id);
		User& findByUsername(const std::string& username);
		size_t size() const;
 };
