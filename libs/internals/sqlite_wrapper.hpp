#include "sqlite3.h"
#include <map>
#include <vector>
#include <string>

namespace internals::sqlite_wrapper {
    typedef std::map<std::string, std::vector<std::string>> sqlite3_table_t;
    
    class sqliteDB
    {
        public:
        sqliteDB();
        ~sqliteDB();
        bool exec(const std::string& stmt);
        bool exec(const std::string& stmt, sqlite3_table_t& result);

        
        private:
        static int api_callback(void *NotUsed, int argc, char **argv, char **azColName);
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        bool malformed_creation;
    };

}