#include <cstdlib>
#include <string>
#include <format>
#include <iostream>
#include "sqlite_wrapper.hpp"

namespace internals::sqlite_wrapper {
sqliteDB::sqliteDB() {
    rc = sqlite3_open(std::format("{}/.config/fitten-banditten/fitten-db.db", std::getenv("HOME")).c_str(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close_v2(db);
    }
    std::cout << "DB initialized" << std::endl;
}

sqliteDB::~sqliteDB() {
    sqlite3_close_v2(db);
}
    
int sqliteDB::api_callback(void *data, int column_count, char **values, char **column_names){
    auto& table = *static_cast<sqlite3_table_t*>(data);

    for(int i = 0; i < column_count; i++){
        table[column_names[i]].push_back(values[i] ? values[i] : "NULL");
    }
    return 0;
}
bool sqliteDB::exec(const std::string& stmt) {
    auto not_used = sqlite3_table_t();
    return exec(stmt, not_used);
}

//TODO(NKC): Use std::expected
bool sqliteDB::exec(const std::string& stmt, sqlite3_table_t& result) {
    rc = sqlite3_exec(db, stmt.c_str(), api_callback, static_cast<void*>(&result), &zErrMsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

}
