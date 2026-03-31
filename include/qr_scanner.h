#ifndef QR_SCANNER_H
#define QR_SCANNER_H

#include <string>
#include <vector>

class QRScanner {
public:
    QRScanner();
    ~QRScanner();

    // Scan QR code from image file
    std::string scanFromFile(const std::string& imagePath);

    // Scan QR code from camera (placeholder for now)
    std::string scanFromCamera();

private:
    // ZXing reader
    void* reader_; // Placeholder for ZXing reader
};

#endif // QR_SCANNER_H