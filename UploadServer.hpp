#ifndef UPLOADSERVER_HPP
#define UPLOADSERVER_HPP

#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UploadServer {
public:
    UploadServer(int port);
    void start();
private:
    void handleClient(int clientSocket);
    int serverSocket;
    struct sockaddr_in serverAddress;
};

#endif
