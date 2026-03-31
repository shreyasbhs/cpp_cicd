#include "qr_scanner.h"
#include <ZXing/ReadBarcode.h>
#include <ZXing/DecodeHints.h>
#include <ZXing/ImageView.h>
#include <opencv2/opencv.hpp>
#include <iostream>

QRScanner::QRScanner() : reader_(nullptr) {
    // Initialize ZXing
}

QRScanner::~QRScanner() {
    // Cleanup
}

std::string QRScanner::scanFromFile(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return "";
    }

    // Convert to ZXing format
    ZXing::ImageView imageView{
        image.data,
        image.cols,
        image.rows,
        ZXing::ImageFormat::BGR
    };

    ZXing::DecodeHints hints;
    hints.setTryHarder(true);

    auto result = ZXing::ReadBarcode(imageView, hints);
    if (result.isValid()) {
        return result.text();
    } else {
        std::cerr << "No QR code found in image." << std::endl;
        return "";
    }
}

std::string QRScanner::scanFromCamera() {
    // Placeholder for camera scanning
    std::cout << "Camera scanning not implemented yet." << std::endl;
    return "";
}