#include "HttpRequest.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h> // For recv
#include <unistd.h>     // For close (if used later)


HttpRequest::HttpRequest(int socket) {
    char buffer[4096];
    recv(socket, buffer, sizeof(buffer), 0);

    std::istringstream requestStream(buffer);
    requestStream >> method >> path;

    std::string line;
    while (std::getline(requestStream, line) && line != "\r") {
        if (line.find(":") != std::string::npos) {
            auto delimiterPos = line.find(":");
            auto key = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 2); // Skip ": "
            headers[key] = value;
        }
    }

    if (headers.count("Content-Length")) {
        int contentLength = std::stoi(headers["Content-Length"]);
        body.resize(contentLength);
        requestStream.read(&body[0], contentLength);
    }
}

std::string HttpRequest::getHeader(const std::string &key) const {
    if (headers.count(key)) return headers.at(key);
    return "";
}
