#include "httplib.h"
#include "sqlite3.h"
#include "../libs/internals/internals.hpp"
#include "../libs/internals/sqlite_wrapper.hpp"
#include <string>
#include <format>

void pages(httplib::Server &svr, std::map<std::string, std::string>& page_contents, internals::sqlite_wrapper::sqliteDB &db) {

    //RETURN userpage.html or login page
    svr.Get("/", [&](const httplib::Request &req, httplib::Response &res) {
        auto result = internals::sqlite_wrapper::sqlite3_table_t();

        std::map<std::string, std::string> cookies;
        getCookiesFromHeader(req.get_header_value("Cookie"), cookies);
        if (cookies.contains("User")) {
            db.exec("SELECT * FROM exercise ORDER BY TYPE", result);
            std::string exercises_html;
            std::cout << result["NAME"].size() << std::endl;
            for (int row = 0; row < result["NAME"].size(); row++) {
                if (result["TYPE"].at(row).contains("0")) {
                    exercises_html += std::format(R"(<label for="{}">{}:</label><br><input type="text" id="{}" name="{}"><br>)", result["NAME"].at(row), result["NAME"].at(row), result["NAME"].at(row), result["NAME"].at(row));
                } else if (result["TYPE"].at(row).contains("1")) {
                    exercises_html += std::format(R"(<label for="{}">{} (hh:mm:ss):</label><br><input id="{}" type="time" step="1" value="00:00:00"><br>)", result["NAME"].at(row), result["NAME"].at(row), result["NAME"].at(row));
                } else {
                    std::cerr << result["NAME"].at(row) << " is of type " << result["TYPE"].at(row) << std::endl;
                }
            } 
            res.set_content(std::vformat(page_contents["userpage"], std::make_format_args(cookies.at("User"), exercises_html)), "text/html");
        } else {
            res.set_content(page_contents["login"], "text/html");
        }
    });
    
    svr.Post("/login", [&](const httplib::Request &req, httplib::Response &res) {
        auto result = internals::sqlite_wrapper::sqlite3_table_t();

        std::string username = req.form.get_field("username");
        db.exec(std::format("SELECT * FROM users WHERE USERNAME IS \"{}\";", username), result);

        if (result["USERNAME"].size() == 0) {
            res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
        } 
        std::cout << "USERNAME : " << result["USERNAME"].at(0) << std::endl; 
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
