#include "HttpServletRequest.hpp"

std::string HttpServletRequest::getHeader(const std::string& headerName) {
    auto it = headers.find(headerName);
    return it != headers.end() ? it->second : "";
}

std::vector<char> HttpServletRequest::getBody() const {
    return body;
}

std::istream& HttpServletRequest::getInputStream() {
    return inputStream;
}
