#include "UploadServer.hpp"
#include "UploadServerThread.hpp"
#include <unistd.h>
#include <stdexcept>
#include <cstdlib>

UploadServer::UploadServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
}

void UploadServer::start() {
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Binding failed");
    }
    listen(serverSocket, 5);

    std::cout << "UploadServer: Server started on port 8082" << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }
        std::thread(&UploadServer::handleClient, this, clientSocket).detach();
    }
}

void UploadServer::handleClient(int clientSocket) {
    UploadServerThread thread(clientSocket);
    thread.run();
}
