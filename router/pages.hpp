#pragma once

void pages(httplib::Server &svr, std::map<std::string, std::string>& page_contents, internals::sqlite_wrapper::sqliteDB &db);

void loadPages(const std::string &base_path, std::map<std::string, std::string>& page_contents);
