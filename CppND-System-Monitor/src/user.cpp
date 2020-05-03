#include <string>
#include "user.h"

User::User(std::string name, std::string password, std::string userID,
        std::string groupID, std::string gecos, std::string homedirectory, std::string shell)
    :name_(name),password_(password) ,userID_(userID),groupID_(groupID),
    gecos_(gecos),homedirectory_(homedirectory),shell_(shell)
{}

std::string User::GetId(){
    return userID_;
}

std::string User::GetUserName(){
    return name_;
}