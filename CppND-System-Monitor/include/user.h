#ifndef USER_H
#define USER_H
#include <string>

class User{
    public:
        User(std::string name, std::string password, std::string userID,
        std::string groupID,std::string gecos, std::string homedirectory, 
        std::string shell);
        User(){};
        std::string GetId();
        std::string GetUserName();

    private:
        std::string name_;
        std::string password_; 
        std::string userID_;
        std::string groupID_;
        std::string gecos_;
        std::string homedirectory_;
        std::string shell_;
};

#endif