#include "db_handler.h"
#include <iostream>
#include <cstring>

DBHandler::DBHandler(const std::string& connectionString)
    : connectionString_(connectionString), henv_(SQL_NULL_HENV),
      hdbc_(SQL_NULL_HDBC), hstmt_(SQL_NULL_HSTMT), connected_(false) {
}

DBHandler::~DBHandler() {
    disconnect();
}

bool DBHandler::connect() {
    if (connected_) return true;

    // Allocate environment handle
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv_) != SQL_SUCCESS) {
        std::cerr << "Failed to allocate environment handle." << std::endl;
        return false;
    }

    // Set ODBC version
    if (SQLSetEnvAttr(henv_, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS) {
        std::cerr << "Failed to set ODBC version." << std::endl;
        return false;
    }

    // Allocate connection handle
    if (SQLAllocHandle(SQL_HANDLE_DBC, henv_, &hdbc_) != SQL_SUCCESS) {
        std::cerr << "Failed to allocate connection handle." << std::endl;
        return false;
    }

    // Connect to database
    SQLCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;
    SQLRETURN ret = SQLDriverConnect(hdbc_, NULL,
        (SQLCHAR*)connectionString_.c_str(), SQL_NTS,
        outConnStr, sizeof(outConnStr), &outConnStrLen, SQL_DRIVER_COMPLETE);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to connect to database." << std::endl;
        return false;
    }

    connected_ = true;
    return true;
}

void DBHandler::disconnect() {
    if (hstmt_ != SQL_NULL_HSTMT) {
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
        hstmt_ = SQL_NULL_HSTMT;
    }
    if (hdbc_ != SQL_NULL_HDBC) {
        SQLDisconnect(hdbc_);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc_);
        hdbc_ = SQL_NULL_HDBC;
    }
    if (henv_ != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv_);
        henv_ = SQL_NULL_HENV;
    }
    connected_ = false;
}

bool DBHandler::insertData(const std::string& qrData) {
    if (!connected_) {
        std::cerr << "Not connected to database." << std::endl;
        return false;
    }

    // Allocate statement handle
    if (SQLAllocHandle(SQL_HANDLE_STMT, hdbc_, &hstmt_) != SQL_SUCCESS) {
        std::cerr << "Failed to allocate statement handle." << std::endl;
        return false;
    }

    // Prepare insert statement
    std::string sql = "INSERT INTO qr_data (data, timestamp) VALUES (?, CURRENT_TIMESTAMP)";
    if (SQLPrepare(hstmt_, (SQLCHAR*)sql.c_str(), SQL_NTS) != SQL_SUCCESS) {
        std::cerr << "Failed to prepare statement." << std::endl;
        return false;
    }

    // Bind parameter
    SQLLEN dataLen = qrData.length();
    if (SQLBindParameter(hstmt_, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
        qrData.length(), 0, (SQLPOINTER)qrData.c_str(), qrData.length(), &dataLen) != SQL_SUCCESS) {
        std::cerr << "Failed to bind parameter." << std::endl;
        return false;
    }

    // Execute
    if (SQLExecute(hstmt_) != SQL_SUCCESS) {
        std::cerr << "Failed to execute statement." << std::endl;
        return false;
    }

    // Free statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
    hstmt_ = SQL_NULL_HSTMT;

    return true;
}