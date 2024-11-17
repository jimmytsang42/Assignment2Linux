#include "HttpServlet.hpp"

void HttpServlet::doGet(HttpServletRequest& request, HttpServletResponse& response) {
    std::cout << "HttpServlet: doGet called" << std::endl;
}

void HttpServlet::doPost(HttpServletRequest& request, HttpServletResponse& response) {
    std::cout << "HttpServlet: doPost called" << std::endl;
}
