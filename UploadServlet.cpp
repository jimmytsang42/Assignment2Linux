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
            <input type='file' name='file'/><br/><br/>
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
        std::vector<char> body = request.getBody();
        std::cout << "UploadServlet Body received: " << std::endl;
        for (char c : body) {
            if (std::isprint(c)) {
                std::cout << c;
            } else {
                std::cout << "\\" << std::hex << (int)(unsigned char)c;
            }
        }
        std::cout << std::endl;

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
                        caption = std::string(content.begin(), content.end());
                    } else if (header.find("name=\"date\"") != std::string::npos) {
                        date = std::string(content.begin(), content.end());
                    } else if (header.find("name=\"file\"") != std::string::npos) {
                        size_t filenameStart = header.find("filename=\"");
                        if (filenameStart != std::string::npos) {
                            filenameStart += 10;
                            size_t filenameEnd = header.find("\"", filenameStart);
                            if (filenameEnd != std::string::npos) {
                                fileName = header.substr(filenameStart, filenameEnd - filenameStart);
                            }
                        }
                        fileContent.assign(content.begin(), content.end());
                    }
                }
            }
        }

        // just for debugging
        std::cout << "Captured caption: " << caption << std::endl;
        std::cout << "Captured date: " << date << std::endl;
        std::cout << "Captured filename: " << fileName << std::endl;
        std::cout << "File content size: " << fileContent.size() << std::endl;

        // Ensure the 'files' directory exists
        std::filesystem::create_directory("files");

        // Construct the new filename and save file
        std::string newFileName = caption + "_" + date + "_" + fileName;
        std::string filePath = "files/" + newFileName;

        // add counter to filename if already exists
        int counter = 1;
        while (std::filesystem::exists(filePath)) {
            filePath = "files/" + caption + "_" + date + "_" + std::to_string(counter++) + "_" + fileName;
        }

        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile) {
            std::cerr << "Error opening file for writing" << std::endl;
            response.writeResponse("Error uploading file.");
            return;
        }
        outFile.write(fileContent.data(), fileContent.size());

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
        htmlResponse << "</ul>";
        htmlResponse << "<p>File uploaded successfully</p>";
        htmlResponse << "<a href='/'>Back to upload</a>";
        htmlResponse << "</body></html>";

        response.setContentType("text/html");
        response.writeResponse(htmlResponse.str());

    } catch (const std::exception& ex) {
        std::cerr << "Error in UploadServlet: " << ex.what() << std::endl;
    }
}

// Helper function to split the multipart body by boundary
std::vector<std::string> UploadServlet::splitMultipart(const std::vector<char>& body, const std::string& boundary) {
    std::vector<std::string> parts;
    std::string bodyStr(body.begin(), body.end());
    size_t start = 0, end;

    while ((end = bodyStr.find("\r\n" + boundary, start)) != std::string::npos) {
        parts.emplace_back(bodyStr.substr(start, end - start));
        start = end + 2 + boundary.length();
    }

    if (start < bodyStr.size()) {
        std::string lastPart = bodyStr.substr(start);
        if (lastPart.find("--") != 0) {
            parts.emplace_back(lastPart);
        }
    }

    return parts;
}

// Helper function to parse headers and content from a multipart part
std::pair<std::vector<std::string>, std::vector<char>> UploadServlet::parseHeadersAndContent(const std::string& part) {
    size_t headerEnd = part.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return {};

    std::string headersPart = part.substr(0, headerEnd);
    std::vector<std::string> headers;
    std::istringstream headerStream(headersPart);
    std::string line;
    while (std::getline(headerStream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        headers.push_back(line);
    }

    std::vector<char> content(part.begin() + headerEnd + 4, part.end());

    std::string contentStr(content.begin(), content.end());
    size_t boundaryPos = contentStr.find("\r\n--");
    if (boundaryPos != std::string::npos) {
        content.erase(content.begin() + boundaryPos, content.end());
    }

    return {headers, content};
}

// Function to split a string by a delimiter
std::vector<std::string> UploadServlet::customsplit(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
