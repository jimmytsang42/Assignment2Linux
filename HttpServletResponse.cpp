#include "HttpServletResponse.hpp"

HttpServletResponse::HttpServletResponse(std::ostream& outputStream)
    : outputStream(outputStream) {}

void HttpServletResponse::setContentType(const std::string& type) {
    contentType = type;
}

void HttpServletResponse::sendResponseHeaders() {
    outputStream << "HTTP/1.1 200 OK" << std::endl;
    if (!contentType.empty()) {
        outputStream << "Content-Type: " << contentType << std::endl;
    }
    outputStream << std::endl;
}

void HttpServletResponse::writeResponse(const std::string& responseBody) {
    sendResponseHeaders();
    outputStream << responseBody;
    outputStream.flush();
}

std::ostream& HttpServletResponse::getOutputStream() {
    return outputStream;
}
