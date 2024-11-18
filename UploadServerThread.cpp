#include "UploadServerThread.hpp"
#include "HttpServletRequest.hpp"
#include "HttpServletResponse.hpp"
#include "HttpServlet.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

#include "UploadServlet.hpp"

UploadServerThread::UploadServerThread(int socket) : socket(socket) {}

void UploadServerThread::run() {
    char buffer[1024];
    ssize_t bytesRead = read(socket, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        std::cerr << "Error reading from socket" << std::endl;
        return;
    }
    buffer[bytesRead] = '\0';

    std::string request(buffer);
    std::cout << "Request: " << request << std::endl;

    std::istringstream requestStream(request);
    std::string method, path;
    requestStream >> method >> path;

    std::map<std::string, std::string> headers;
    std::string headerLine;
    while (std::getline(requestStream, headerLine) && !headerLine.empty()) {
        size_t delimiterPos = headerLine.find(": ");
        if (delimiterPos != std::string::npos) {
            std::string headerName = headerLine.substr(0, delimiterPos);
            std::string headerValue = headerLine.substr(delimiterPos + 2);
            headers[headerName] = headerValue;
        }
    }

    std::vector<char> body;
    if (method == "POST" && headers.find("Content-Length") != headers.end()) {
        size_t contentLength = std::stoi(headers["Content-Length"]);
        body.resize(contentLength);
        ssize_t totalBytesRead = 0;
        while (totalBytesRead < contentLength) {
            ssize_t bytesRead = read(socket, &body[totalBytesRead], contentLength - totalBytesRead);
            if (bytesRead < 0) {
                return; // abort
            }
            totalBytesRead += bytesRead;
        }
    }

    HttpServletRequest requestObj(std::cin, headers, body);
    std::ostringstream responseStream;
    HttpServletResponse responseObj(responseStream);

    HttpServlet* servlet = nullptr;

    servlet = new UploadServlet();

    if (method == "POST") {
        servlet->doPost(requestObj, responseObj);
    } else if (method == "GET") {
        servlet->doGet(requestObj, responseObj);
    }

    std::string response = responseStream.str();
    write(socket, response.c_str(), response.size());
    close(socket);
}
