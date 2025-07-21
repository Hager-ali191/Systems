#pragma once
#include <string>
#include <iostream>

class User
{
protected:
    std::string username;
    std::string password;

public:
    User(const std::string &uname, const std::string &pwd)
        : username(uname), password(pwd) {}

    virtual bool login(const std::string &uname, const std::string &pwd) = 0;
    virtual void showMenu() = 0;
    virtual void performAction(int choice) = 0;
    virtual ~User() {}
};