#include "HttpServletRequest.hpp"

HttpServletRequest::HttpServletRequest(std::istream& inputStream, const std::map<std::string, std::string>& headers, const std::string& body)
    : inputStream(inputStream), headers(headers), body(body) {}

std::string HttpServletRequest::getHeader(const std::string& headerName) {
    auto it = headers.find(headerName);
    return it != headers.end() ? it->second : "";
}

std::string HttpServletRequest::getBody() {
    return body;
}

std::istream& HttpServletRequest::getInputStream() {
    return inputStream;
}
