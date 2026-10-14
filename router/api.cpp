#include "httplib.h"
#include "sqlite3.h"
#include "../libs/internals/sqlite_wrapper.hpp"
#include "api.hpp"

void api(httplib::Server &svr, std::map<std::string, std::string>& page_contents, internals::sqlite_wrapper::sqliteDB &db) {

    svr.Post("/api/adduser", [&](const auto& req, auto& res) {
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

    svr.Post("/exercise/add", [&](const auto& req, auto& res) {
        std::string exercise_name = req.form.get_field("exercise");
        std::string exercise_type = req.form.get_field("etype");

        std::cout << exercise_name << " : " << exercise_type << std::endl;

        res.set_content(page_contents["exercise-add"], "text/html");

        
        //TODO(NKC): use wildcard insert in the future
        //std::string stmt = std::format("INSERT INTO users (USERNAME, PASSWORD_HASH) VALUES('{}', '{}');", username, password);
        //std::cout << stmt << std::endl;
        //rc = sqlite3_exec(db, stmt.c_str(), api_callback, 0, &zErrMsg);
        //if( rc!=SQLITE_OK ){
        //    fprintf(stderr, "SQL error: %s\n", zErrMsg);
        //    sqlite3_free(zErrMsg);
        //}

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

}