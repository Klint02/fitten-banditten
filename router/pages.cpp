#include "httplib.h"
#include "sqlite3.h"
#include "../libs/internals/internals.hpp"
#include <string>
#include <format>

void pages(httplib::Server &svr, std::map<std::string, std::string>& page_contents) {

    //RETURN userpage.html or login page
    svr.Get("/", [&](const httplib::Request &req, httplib::Response &res) {
        std::map<std::string, std::string> cookies;
        getCookiesFromHeader(req.get_header_value("Cookie"), cookies);
        if (cookies.contains("User")) {
            res.set_content(std::vformat(page_contents["userpage"], std::make_format_args(cookies.at("User"))), "text/html");
        } else {
            res.set_content(page_contents["login"], "text/html");
        }
    });
    
    svr.Post("/login", [&](const httplib::Request &req, httplib::Response &res) {
        std::string username = req.form.get_field("username");
        res.set_header("Set-Cookie", "User=" + username);
        res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
    });

    svr.Get("/exercise/page", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_content(page_contents["exercise-add"], "text/html");
    });
    
}

void loadPages(const std::string& base_path, std::map<std::string, std::string>& page_contents) {

    std::vector<std::string> pages = {"userpage", "login", "redirect", "exercise-add"};

    for (const auto& page : pages) {

        std::string content;
        std::string line;
        std::string page_file_path = base_path + "/" + page + ".html";
        std::ifstream file (page_file_path);

        if (file.is_open())
        {
            while ( getline (file,line) )
            {
                content += line; 
                content += "\n";
            }
            file.close();

            page_contents[page] = content;
            //std::cout << root_content << std::endl;
        } else {
            std::cout << "Unable to open file: " << page_file_path << std::endl;
        }
    }
}
