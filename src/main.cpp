#include "qr_scanner.h"
#include "db_handler.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <image_path> <connection_string>" << std::endl;
        std::cout << "Example connection string: DRIVER={IBM INFORMIX ODBC DRIVER};SERVER=server_name;DATABASE=db_name;HOST=host;SERVICE=service;PROTOCOL=protocol;UID=user;PWD=password;" << std::endl;
        return 1;
    }

    std::string imagePath = argv[1];
    std::string connectionString = argv[2];

    // Initialize QR scanner
    QRScanner scanner;

    // Scan QR code
    std::string qrData = scanner.scanFromFile(imagePath);
    if (qrData.empty()) {
        std::cerr << "Failed to scan QR code." << std::endl;
        return 1;
    }

    std::cout << "Scanned QR data: " << qrData << std::endl;

    // Initialize database handler
    DBHandler dbHandler(connectionString);

    // Connect to database
    if (!dbHandler.connect()) {
        std::cerr << "Failed to connect to database." << std::endl;
        return 1;
    }

    // Insert data
    if (dbHandler.insertData(qrData)) {
        std::cout << "Data inserted successfully." << std::endl;
    } else {
        std::cerr << "Failed to insert data." << std::endl;
        return 1;
    }

    return 0;
}