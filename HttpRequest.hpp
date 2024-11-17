#pragma once
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>

class HttpRequest {
    std::string method;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;

public:
    explicit HttpRequest(int socket);
    std::string getMethod() const { return method; }
    std::string getPath() const { return path; }
    std::string getHeader(const std::string &key) const;
    std::string getBody() const { return body; }
};
