#ifndef HTTPSERVLET_HPP
#define HTTPSERVLET_HPP

#include <string>
#include <iostream>
#include "HttpServletRequest.hpp"
#include "HttpServletResponse.hpp"

class HttpServlet {
public:
    virtual void doGet(HttpServletRequest& request, HttpServletResponse& response);
    virtual void doPost(HttpServletRequest& request, HttpServletResponse& response);
    virtual ~HttpServlet() = default;
};

#endif
