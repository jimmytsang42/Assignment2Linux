Upload Server for Assignment 2 in C++/Linux

Note: the following steps below assume a windows installation.

Clone this repo.

Install cygwin https://www.cygwin.com/

In cygwin type: cd /cygdrive/path/to/tomcat/webapps/assignment2linux

Then in cygwin, type this command to compile the files.
$ g++ -std=c++20 -o UploadServer *.cpp

Then type this in cygwin to start the server.
$ ./UploadServer

Go to localhost:8082 in a browser to upload a file.