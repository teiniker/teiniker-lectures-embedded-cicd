#include "user.h"

using namespace std;

User::User(const int id, const string& username, const string& password)
	: _id{id}, _username{username}, _password{password}
{
}

