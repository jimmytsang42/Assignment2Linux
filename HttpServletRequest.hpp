#ifndef HTTPSERVLETREQUEST_HPP
#define HTTPSERVLETREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

class HttpServletRequest {
public:
    HttpServletRequest(std::istream& inputStream, const std::map<std::string, std::string>& headers, const std::string& body);
    std::string getHeader(const std::string& headerName);
    std::string getBody();
    std::istream& getInputStream();

private:
    std::istream& inputStream;
    std::map<std::string, std::string> headers;
    std::string body;
};

#endif
