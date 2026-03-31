# QR Code Scanner and Informix Database Storage

This C++ project scans QR codes from images and stores the data in an Informix database.

## Dependencies

- CMake 3.10+
- ZXing C++ library
- OpenCV
- ODBC driver for Informix
- IBM Informix Client SDK

## Installation

### Ubuntu/Debian

```bash
# Install build tools
sudo apt update
sudo apt install cmake build-essential

# Install OpenCV
sudo apt install libopencv-dev

# Install ODBC
sudo apt install unixodbc-dev

# Install ZXing (if available via apt, otherwise build from source)
sudo apt install libzxing-dev

# For Informix, download and install IBM Informix Client SDK
# Follow IBM's instructions for installation
```

### Building ZXing from source (if not available via package manager)

```bash
git clone https://github.com/zxing-cpp/zxing-cpp.git
cd zxing-cpp
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./qr_scanner <image_path> "<connection_string>"
```

Example:

```bash
./qr_scanner qr_code.png "DRIVER={IBM INFORMIX ODBC DRIVER};SERVER=myserver;DATABASE=mydb;HOST=localhost;SERVICE=9088;PROTOCOL=onsoctcp;UID=myuser;PWD=mypassword;"
```

## Database Setup

Create a table in your Informix database:

```sql
CREATE TABLE qr_data (
    id SERIAL PRIMARY KEY,
    data VARCHAR(255),
    timestamp DATETIME YEAR TO SECOND
);
```

## Notes

- Ensure the Informix ODBC driver is properly configured in odbcinst.ini
- The connection string format may vary depending on your Informix setup
- Camera scanning functionality is not implemented yet

## CI/CD

This project includes a GitHub Actions workflow that automatically builds the project on pushes and pull requests to the main/master branch. The built executable is then uploaded to a remote server via SCP.

### Required GitHub Secrets

To enable artifact deployment, set the following secrets in your GitHub repository settings:

- `SERVER_IP`: The IP address of the deployment server
- `SERVER_USER`: The SSH username for the server
- `SSH_PRIVATE_KEY`: The private SSH key for authentication (generate with `ssh-keygen` and add the public key to the server's `~/.ssh/authorized_keys`)

The workflow will upload the `qr_scanner` executable to `/home/${SERVER_USER}/artifacts/` on the server.