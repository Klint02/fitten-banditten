#include <string>
#include <map>
#include <iostream>
#include <openssl/evp.h>
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

std::string SHA512hashPassword(const std::string &password) {
    unsigned char buffer[EVP_MAX_MD_SIZE];
    size_t digest_len = 0;
    // int EVP_Q_digest(OSSL_LIB_CTX *libctx, const char *name, const char *propq, const void *data, size_t datalen, unsigned char *md, size_t *mdlen);
    
    if (EVP_Q_digest(nullptr,"SHA512", nullptr, password.data(), password.size(), buffer, &digest_len) != 1) {
        std::cerr << "Password hashing failed" << std::endl;
    }
    
    constexpr char digits[] = "0123456789abcdef";

    std::span<const unsigned char> digest_span{buffer, digest_len};
    std::string result;
    result.reserve(digest_span.size() * 2);

    for (unsigned char byte : digest_span) {
        result.push_back(digits[byte >> 4]);
        result.push_back(digits[byte & 0x0f]);
    }

    return result;

}
