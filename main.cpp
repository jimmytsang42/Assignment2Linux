#include <iostream>
#include <thread>
#include <vector>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "FileHandler.hpp"

#define PORT 8082

void handleClient(int clientSocket) {
    try {
        // Parse request
        HttpRequest request(clientSocket);
        HttpResponse response(clientSocket);

        if (request.getMethod() == "POST") {
            FileHandler fileHandler;
            fileHandler.saveFile(request);
            response.sendFileList("files/"); // Respond with the list of uploaded files
        } else if (request.getMethod() == "GET") {
            response.sendHTMLForm(); // Serve an HTML form for file upload
        } else {
            response.sendMethodNotAllowed();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    close(clientSocket);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << PORT << std::endl;

    std::vector<std::thread> threads;
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }
        threads.emplace_back(handleClient, clientSocket);
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    close(serverSocket);
    return 0;
}
