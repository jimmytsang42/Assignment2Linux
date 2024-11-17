#ifndef HTTPSERVLETVIEW_HPP
#define HTTPSERVLETVIEW_HPP

#include <string>
#include <iostream>
#include <sstream>

class HttpServletResponse {
public:
    HttpServletResponse(std::ostream& outputStream);
    void setContentType(const std::string& type);
    void sendResponseHeaders();
    void writeResponse(const std::string& responseBody);
    std::ostream& getOutputStream();

private:
    std::ostream& outputStream;
    std::string contentType;
};

#endif
