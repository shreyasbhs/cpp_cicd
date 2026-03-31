#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <string>
#include <sql.h>
#include <sqlext.h>

class DBHandler {
public:
    DBHandler(const std::string& connectionString);
    ~DBHandler();

    bool connect();
    void disconnect();
    bool insertData(const std::string& qrData);

private:
    std::string connectionString_;
    SQLHENV henv_;
    SQLHDBC hdbc_;
    SQLHSTMT hstmt_;
    bool connected_;
};

#endif // DB_HANDLER_H