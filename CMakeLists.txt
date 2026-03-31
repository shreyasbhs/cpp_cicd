cmake_minimum_required(VERSION 3.10)
project(QRCodeScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZXING REQUIRED zxing)
find_package(ODBC REQUIRED)

# Include directories
include_directories(include)
include_directories(${ZXING_INCLUDE_DIRS})
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