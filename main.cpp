
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include "libs/internals/sqlite_wrapper.hpp"
#include <httplib.h>
#include "router/pages.hpp"
#include "router/api.hpp"

int main(int, char**){
    auto db = internals::sqlite_wrapper::sqliteDB();

    std::cout << "Hello, from fitten-banditten!\n";
    // HTTP
    httplib::Server svr;
    
    std::string statics_path = std::format("{}/.config/fitten-banditten/static", std::getenv("HOME"));

    auto ret = svr.set_mount_point("/static", statics_path);
    if (!ret) {
        std::cout << "Unable to mount static folder: " << statics_path <<  std::endl;
        return -1;
    } else {
        std::cout << "mounted static folder: " << statics_path << std::endl;
    }

    std::map<std::string, std::string> page_contents;

    loadPages(statics_path,  page_contents);
    pages(svr, page_contents, db);
    api(svr, page_contents, db);

    std::cout << "hmmm" << std::endl;
    svr.listen("0.0.0.0", 8080);

}
