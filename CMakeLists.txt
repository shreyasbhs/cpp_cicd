cmake_minimum_required(VERSION 3.10)
project(QRCodeScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZXING REQUIRED zxing)
find_package(ODBC REQUIRED)

# Ensure ZXing headers are available (search standard and /usr/local paths)
find_path(ZXING_INCLUDE_PATH ZXing/DecodeHints.h
  PATHS /usr/local/include /usr/include ${ZXING_INCLUDE_DIRS}
  NO_DEFAULT_PATH)
if(NOT ZXING_INCLUDE_PATH)
    find_path(ZXING_INCLUDE_PATH ZXing/DecodeHints.h)
endif()
if(NOT ZXING_INCLUDE_PATH)
    message(FATAL_ERROR "ZXing headers not found (ZXing/DecodeHints.h). Install zxing-cpp and rerun CMake.")
endif()

# Include directories
include_directories(include)
include_directories(${ZXING_INCLUDE_DIRS})
include_directories(${ZXING_INCLUDE_PATH})
include_directories(${ODBC_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/qr_scanner.cpp
    src/db_handler.cpp
)

# Executable
add_executable(qr_scanner ${SOURCES})

# Link libraries
target_link_libraries(qr_scanner ${ZXING_LIBRARIES} ${ODBC_LIBRARIES})

# Compiler flags
target_compile_options(qr_scanner PRIVATE ${ZXING_CFLAGS_OTHER})