#ifndef UPLOADSERVLET_HPP
#define UPLOADSERVLET_HPP

#include <algorithm>

#include "HttpServlet.hpp"
#include <string>
#include <vector>

class UploadServlet : public HttpServlet {
public:
    void doGet(HttpServletRequest& request, HttpServletResponse& response) override;
    void doPost(HttpServletRequest& request, HttpServletResponse& response) override;

    std::vector<std::string> splitMultipart(const std::string &body, const std::string &boundary);

    std::pair<std::vector<std::string>, std::string> parseHeadersAndContent(const std::string &part);

    std::vector<std::string> customsplit(const std::string &str, char delimiter);
};

#endif
