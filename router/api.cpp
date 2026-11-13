#include "httplib.h"
#include "sqlite3.h"
#include "../libs/internals/sqlite_wrapper.hpp"
#include "../libs/internals/internals.hpp"
#include <string>
#include "api.hpp"

void api(httplib::Server &svr, std::map<std::string, std::string>& page_contents, internals::sqlite_wrapper::sqliteDB &db) {

    svr.Post("/auth/user", [&](const httplib::Request &req, httplib::Response &res) {
        auto result = internals::sqlite_wrapper::sqlite3_table_t();

        std::string username = req.form.get_field("username");
        std::string password = req.form.get_field("password"); //TODO: Implement password hashes

        std::string hash = SHA512hashPassword(username + password);


        db.exec(std::format("SELECT USERNAME FROM users WHERE USERNAME IS \"{}\" AND PASSWORD_HASH IS \"{}\";", username, hash), result);

        if (result["USERNAME"].size() == 0) {
            res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
        } 
        std::cout << "USERNAME : " << result["USERNAME"].at(0) << std::endl; 
        //NOTE: Path is set to "/" so we can access the user across all paths
        res.set_header("Set-Cookie", "User=" + username + "; Path=/");
        res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
    });

    svr.Post("/add/user", [&](const auto& req, auto& res) {
        std::string username = req.form.get_field("username");
        std::string password = req.form.get_field("password");
        
        std::string hash = SHA512hashPassword(username + password);

        //Safety so password isn't in memory anymore 
        password = "";
        
        if (username.size() == 0 && password.size() != 0) {
            res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/signup")), "text/html");
        } else {
            //TODO(NKC): use wildcard insert in the future
            std::string stmt = std::format("INSERT INTO users (USERNAME, PASSWORD_HASH) VALUES('{}', '{}');", username, hash);
            std::cout << stmt << std::endl;
            db.exec(stmt);
            res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
        }

        
    });
    
    svr.Post("/add/exercise", [&](const auto& req, auto& res) {
        std::string exercise_name = req.form.get_field("exercise");
        std::string exercise_type = req.form.get_field("etype");

        std::string stmt = std::format("INSERT INTO exercise (NAME, TYPE) VALUES('{}', '{}');", exercise_name, exercise_type);
        //TODO(nkc): Error handling
        db.exec(stmt);
        res.set_content(std::vformat(page_contents["redirect"], std::make_format_args("/")), "text/html");
    });
    
    /*
    svr.Post("/api/addtag", [&](const auto& req, auto& res) {
        std::string username = req.form.get_field("username");
        std::string tag = req.form.get_field("tag");
        
        if (tag.empty()) {
            return;
        }

        if (!username.contains("klint")) {
            std::cerr << "user " << username << " tried to add tag " << tag << std::endl; 
            return;
        }

        //TODO(NKC): use wildcard insert in the future
        std::string stmt = std::format("INSERT INTO TAGS (NAME) VALUES('{}');", tag);
        std::cout << stmt << std::endl;
        db.exec(stmt);
    });



        svr.Post("/api/linkexercise", [&](const auto& req, auto& res) {
        std::string username = req.get_param_value("username");
        std::string password = req.get_param_value("password");

        // Check existence
        if (req.has_param("username")) {
            username = req.get_param_value("username");
        }

        if (req.has_param("password")) {
            password = req.get_param_value("password");
        }

        //TODO(NKC): use wildcard insert in the future
        std::string stmt = std::format("INSERT INTO users (USERNAME, PASSWORD_HASH) VALUES('{}', '{}');", username, password);
        std::cout << stmt << std::endl;
        db.exec(stmt);
    });

    svr.Get("/api/fetch_exercise_by_tag", [&](const auto& req, auto& res) {
        std::string username = req.get_param_value("username");
        std::string password = req.get_param_value("password");

        // Check existence
        if (req.has_param("username")) {
            username = req.get_param_value("username");
        }

        if (req.has_param("password")) {
            password = req.get_param_value("password");
        }

        //TODO(NKC): use wildcard insert in the future
        std::string stmt = std::format("INSERT INTO users (USERNAME, PASSWORD_HASH) VALUES('{}', '{}');", username, password);
        std::cout << stmt << std::endl;
        db.exec(stmt);
    });
    */

}