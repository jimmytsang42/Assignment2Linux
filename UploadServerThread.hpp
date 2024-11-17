#ifndef UPLOADSERVERTHREAD_HPP
#define UPLOADSERVERTHREAD_HPP

#include <iostream>
#include <map>
#include <string>

class UploadServerThread {
public:
    UploadServerThread(int socket);
    void run();
private:
    int socket;
};

#endif
