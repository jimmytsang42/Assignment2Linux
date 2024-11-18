#ifndef HTTPSERVLETREQUEST_HPP
#define HTTPSERVLETREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class HttpServletRequest {
public:
    HttpServletRequest(std::istream& inputStream, const std::map<std::string, std::string>& headers, const std::vector<char>& body)
        : inputStream(inputStream), headers(headers), body(body) {}
    std::string getHeader(const std::string& headerName);
    std::vector<char> getBody() const;
    std::istream& getInputStream();

private:
    std::istream& inputStream;
    std::map<std::string, std::string> headers;
    std::vector<char> body;
};

#endif
