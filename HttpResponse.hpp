#pragma once
#include <string>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

class HttpResponse {
    int clientSocket;

public:
    explicit HttpResponse(int socket);
    void sendHTMLForm();
    void sendFileList(const std::string &directoryPath);
    void sendMethodNotAllowed();

private:
    void sendResponse(const std::string &status, const std::string &contentType, const std::string &body);
};
