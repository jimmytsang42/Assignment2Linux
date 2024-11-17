#include "UploadServlet.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

void UploadServlet::doGet(HttpServletRequest& request, HttpServletResponse& response) {
    std::cout << "UploadServlet: doGet method invoked" << std::endl;
    response.setContentType("text/html");
    std::string htmlResponse = R"(
        <!DOCTYPE html>
        <html>
        <body>
        <h2>HTML Forms from UploadServlet</h2>
        <form method='post' enctype='multipart/form-data'>
            Caption: <input type='text' name='caption'/><br/><br/>
            Date: <input type='date' name='date' /><br/>
            <input type='file' name='fileName'/><br/><br/>
            <input type='submit' value='Submit' />
        </form>
        </body>
        </html>
    )";
    response.writeResponse(htmlResponse);
}

void UploadServlet::doPost(HttpServletRequest& request, HttpServletResponse& response) {
    std::cout << "UploadServlet: doPost method invoked" << std::endl;

    try {
        std::string body = request.getBody();
        std::cout << "UploadServlet Body received: " << body << std::endl;

        // Get boundary from Content-Type header
        std::string contentType = request.getHeader("Content-Type");
        std::vector<std::string> contentParts = customsplit(contentType, '=');
        std::string boundary = "--" + contentParts[1];
        std::vector<std::string> parts = splitMultipart(body, boundary);

        // Form data
        std::string caption, date, fileName;
        std::vector<char> fileContent;

        // Parse parts
        for (const std::string& part : parts) {
            if (part.empty() || part == "--") continue;

            auto [headers, content] = parseHeadersAndContent(part);
            if (headers.empty() || content.empty()) continue;

            // Check for Content-Disposition header to identify fields and files
            for (const std::string& header : headers) {
                if (header.find("Content-Disposition") != std::string::npos) {
                    if (header.find("name=\"caption\"") != std::string::npos) {
                        caption = content;
                    } else if (header.find("name=\"date\"") != std::string::npos) {
                        date = content;
                    } else if (header.find("filename=\"") != std::string::npos) {
                        fileName = content;
                        fileContent.assign(content.begin(), content.end());
                    }
                }
            }
        }

        // Ensure the 'files' directory exists
        std::filesystem::create_directory("files");

        // Construct the new filename and save file
        std::string newFileName = caption + "_" + date + "_" + fileName;
        std::ofstream outFile("files/" + newFileName, std::ios::binary);
        outFile.write(fileContent.data(), fileContent.size());
        std::cout << "UploadServlet: File saved as " << newFileName << std::endl;

        // Generate a sorted listing of the files
        std::vector<std::string> fileNames;
        for (const auto& entry : std::filesystem::directory_iterator("files")) {
            if (entry.is_regular_file()) {
                fileNames.push_back(entry.path().filename().string());
            }
        }
        std::sort(fileNames.begin(), fileNames.end());

        // Prepare the HTML response to show uploaded files
        std::ostringstream htmlResponse;
        htmlResponse << "<!DOCTYPE html><html><body><h2>Uploaded Files</h2><ul>";
        for (const std::string& name : fileNames) {
            htmlResponse << "<li>" << name << "</li>";
        }
        htmlResponse << "</ul></body></html>";

        response.setContentType("text/html");
        response.writeResponse(htmlResponse.str());

    } catch (const std::exception& ex) {
        std::cerr << "Error in UploadServlet: " << ex.what() << std::endl;
    }
}

// Helper function to split the multipart body by boundary
std::vector<std::string> UploadServlet::splitMultipart(const std::string& body, const std::string& boundary) {
    std::vector<std::string> parts;
    size_t start = 0;
    while ((start = body.find(boundary, start)) != std::string::npos) {
        size_t end = body.find(boundary, start + boundary.length());
        if (end == std::string::npos) break;
        parts.push_back(body.substr(start + boundary.length(), end - start - boundary.length()));
        start = end;
    }
    return parts;
}

// Helper function to parse headers and content from a multipart part
std::pair<std::vector<std::string>, std::string> UploadServlet::parseHeadersAndContent(const std::string& part) {
    std::vector<std::string> headers;
    std::string content;

    size_t headerEnd = part.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return {};

    headers.push_back(part.substr(0, headerEnd));
    content = part.substr(headerEnd + 4); // Skip the "\r\n\r\n"

    return {headers, content};
}

// Function to split a string by a delimiter
std::vector<std::string> UploadServlet:: customsplit(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}