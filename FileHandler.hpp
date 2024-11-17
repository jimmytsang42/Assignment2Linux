#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

class FileHandler {
public:
    // Constructor to set the directory path
    explicit FileHandler(const std::string& directoryPath)
        : directoryPath(directoryPath) {
        // Create the directory if it does not exist
        if (!fs::exists(directoryPath)) {
            fs::create_directory(directoryPath);
            std::cout << "FileHandler: Created directory " << directoryPath << std::endl;
        }
    }

    // Save a file with given content and name
    bool saveFile(const std::string& fileName, const std::vector<uint8_t>& fileContent) {
        try {
            std::string filePath = directoryPath + "/" + fileName;
            std::ofstream outFile(filePath, std::ios::binary);

            if (!outFile.is_open()) {
                std::cerr << "FileHandler: Failed to open file " << filePath << " for writing." << std::endl;
                return false;
            }

            outFile.write(reinterpret_cast<const char*>(fileContent.data()), fileContent.size());
            outFile.close();

            std::cout << "FileHandler: File saved as " << filePath << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception occurred while saving file: " << e.what() << std::endl;
            return false;
        }
    }

    // List all files in the directory, sorted by name
    std::vector<std::string> listFiles() const {
        std::vector<std::string> fileNames;

        try {
            for (const auto& entry : fs::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    fileNames.push_back(entry.path().filename().string());
                }
            }

            std::sort(fileNames.begin(), fileNames.end());
        } catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception occurred while listing files: " << e.what() << std::endl;
        }

        return fileNames;
    }

    // Delete a file by name
    bool deleteFile(const std::string& fileName) {
        try {
            std::string filePath = directoryPath + "/" + fileName;
            if (fs::remove(filePath)) {
                std::cout << "FileHandler: File " << filePath << " deleted successfully." << std::endl;
                return true;
            } else {
                std::cerr << "FileHandler: File " << filePath << " not found." << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception occurred while deleting file: " << e.what() << std::endl;
            return false;
        }
    }

    // Get the full path of the directory
    std::string getDirectoryPath() const {
        return directoryPath;
    }

private:
    std::string directoryPath;
};

#endif // FILEHANDLER_HPP
