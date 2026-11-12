#pragma once

#include <string>

class User 
{
	private:
		int _id; 
		std::string _username;
		std::string _password;

	public:                
		User(const int id, const std::string& username, const std::string& password);

		int id() const { return _id; }
		void set_id(const int id) { _id = id; }
		
		std::string username(void) const { return _username; }
		void set_username(const std::string& username) { _username = username; }
		
		std::string password() const { return _password; }
		void set_password(const std::string& password) { _password = password; }
 };
