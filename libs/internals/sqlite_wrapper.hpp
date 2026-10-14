#include "sqlite3.h"

namespace internals::sqlite_wrapper {
    class sqliteDB
    {
        public:
        sqliteDB();
        ~sqliteDB();
        bool exec(const std::string& stmt);

        
        private:
        static int api_callback(void *NotUsed, int argc, char **argv, char **azColName);
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        bool malformed_creation;
    };
}