#pragma once

void pages(httplib::Server &svr, std::map<std::string, std::string>& page_contents);

void loadPages(const std::string &base_path, std::map<std::string, std::string>& page_contents);
