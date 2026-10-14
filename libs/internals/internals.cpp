#include <string>
#include <map>
#include <iostream>
#include "internals.hpp"


void getCookiesFromHeader(const std::string &header, std::map<std::string, std::string> &cookies) {
    std::string raw_cookies = header;
    std::cout << header << std::endl;
    if (header.size() < 1)
        return;

    int sub_str_start = 0;
    int sub_str_end = header.substr(sub_str_start).find_first_of(";");
    int it = 0;
    bool parsed = false;
    
    while (!parsed) {
        if (sub_str_end == std::string::npos)
            parsed = true;
        
        std::string raw_cookie = header.substr(sub_str_start, sub_str_end);
        int delimiter = raw_cookie.find_last_of("=");
        cookies[raw_cookie.substr(0, delimiter)] = raw_cookie.substr(delimiter + 1);
        sub_str_start = sub_str_start + sub_str_end + 2;
        sub_str_end = header.substr(sub_str_start).find_first_of(";");

    }

    for (const auto& [key, val] : cookies) {
        std::cout << key << " : " << val << std::endl;
    }
}
