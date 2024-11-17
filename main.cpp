#include "UploadServer.hpp"

int main() {
    try {
        UploadServer server(8082);
        server.start();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
