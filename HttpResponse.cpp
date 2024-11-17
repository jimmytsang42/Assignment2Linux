#include "httpresponse.hpp"

HttpResponse::HttpResponse(int socket) : clientSocket(socket) {}

// Helper function to send a complete HTTP response
void HttpResponse::sendResponse(const std::string &status, const std::string &contentType, const std::string &body) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << body;

    std::string responseStr = response.str();
    if (write(clientSocket, responseStr.c_str(), responseStr.size()) == -1) {
        std::cerr << "HttpResponse: Failed to send response." << std::endl;
    }
}

// Sends an HTML form as the response
void HttpResponse::sendHTMLForm() {
    std::string body = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>File Upload</title>
        </head>
        <body>
            <h1>Upload a File</h1>
            <form method="POST" enctype="multipart/form-data" action="/upload">
                <input type="file" name="file" required>
                <button type="submit">Upload</button>
            </form>
        </body>
        </html>
    )";

    sendResponse("200 OK", "text/html", body);
}

// Sends a list of files in the specified directory
void HttpResponse::sendFileList(const std::string &directoryPath) {
    std::ostringstream body;
    body << R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>File List</title>
        </head>
        <body>
            <h1>Files in Directory</h1>
            <ul>
    )";

    try {
        for (const auto &entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                body << "<li>" << entry.path().filename().string() << "</li>";
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "HttpResponse: Exception occurred while reading directory: " << e.what() << std::endl;
        sendResponse("500 Internal Server Error", "text/plain", "Error reading directory.");
        return;
    }

    body << R"(
            </ul>
        </body>
        </html>
    )";

    sendResponse("200 OK", "text/html", body.str());
}

// Sends a 405 Method Not Allowed response
void HttpResponse::sendMethodNotAllowed() {
    std::string body = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>405 Method Not Allowed</title>
        </head>
        <body>
            <h1>405 Method Not Allowed</h1>
            <p>The requested method is not allowed on this server.</p>
        </body>
        </html>
    )";

    sendResponse("405 Method Not Allowed", "text/html", body);
}
