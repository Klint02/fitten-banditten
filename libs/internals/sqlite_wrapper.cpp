#include <cstdlib>
#include <string>
#include <format>
#include <iostream>
#include "sqlite_wrapper.hpp"

namespace internals::sqlite_wrapper {
sqliteDB::sqliteDB() {
    rc = sqlite3_open(std::format("{}/.config/fitten-db.db", std::getenv("HOME")).c_str(), &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close_v2(db);
    }
    std::cout << "DB initialized" << std::endl;
}

sqliteDB::~sqliteDB() {
    sqlite3_close_v2(db);
}
    
int sqliteDB::api_callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

//TODO(NKC): Use std::expected
bool sqliteDB::exec(const std::string& stmt) {
    rc = sqlite3_exec(db, stmt.c_str(), api_callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

}
