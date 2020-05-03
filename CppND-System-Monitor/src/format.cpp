#include <string>
#include "format.h"
#include <iostream>  
#include <sstream>  
#include <ctime>
#include <cmath>
#include <iomanip>
using std::string;
using std::stringstream;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    auto hours = floor(seconds/3600);
    auto mins = floor((seconds - hours*3600)/60);
    auto secs = floor(((seconds - hours*3600) - mins*60));
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":" 
        << std::setw(2) << std::setfill('0') << mins << ":" 
        << std::setw(2) << std::setfill('0') << secs;
    string s;
    ss >> s;
    return s; 
    }