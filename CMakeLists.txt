cmake_minimum_required(VERSION 3.29)
project(Assignment2Linux)

set(CMAKE_CXX_STANDARD 20)

add_executable(Assignment2Linux main.cpp
        HttpRequest.cpp
        HttpRequest.hpp
        HttpResponse.cpp
        HttpResponse.hpp
        FileHandler.cpp
        FileHandler.hpp
)
