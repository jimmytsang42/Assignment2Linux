cmake_minimum_required(VERSION 3.29)
project(Assignment2Linux)

set(CMAKE_CXX_STANDARD 20)

add_executable(Assignment2Linux main.cpp
        HttpServlet.cpp
        HttpServlet.hpp
        HttpServlet.hpp
        main.cpp
        HttpServletRequest.cpp
        HttpServletRequest.hpp
        HttpServletResponse.cpp
        HttpServletResponse.hpp
        UploadServer.cpp
        UploadServer.hpp
        UploadServerThread.cpp
        UploadServerThread.hpp
        UploadServlet.cpp
        UploadServlet.hpp

)
