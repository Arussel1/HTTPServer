#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctime>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

std::unordered_map<std::string, std::string> mimeTypes = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".gif", "image/gif"},
    {".json", "application/json"},
};

int PORT = 8080;
std::string ROOT = "public";

std::unordered_map<std::string, std::string> fileCache;

void loadConfig() {
    std::ifstream configFile("config/server_config.txt");
    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("PORT=") == 0) {
                PORT = std::stoi(line.substr(5));
            } else if (line.find("ROOT=") == 0) {
                ROOT = line.substr(5);
            }
        }
        configFile.close();
    } else {
        std::cerr << "Unable to open config file. Using default settings." << std::endl;
    }
}

std::string getMimeType(const std::string& extension) {
    return mimeTypes.count(extension) ? mimeTypes[extension] : "application/octet-stream";
}

void logRequest(const std::string& request) {
    std::ofstream logFile("server.log", std::ios_base::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        logFile << std::ctime(&now) << ": " << request << std::endl;
        logFile.close();
    }
}

std::unordered_map<std::string, std::string> parseQueryParams(const std::string& url) {
    std::unordered_map<std::string, std::string> params;
    size_t pos = url.find('?');
    if (pos != std::string::npos) {
        std::string queryString = url.substr(pos + 1);
        std::istringstream queryStream(queryString);
        std::string param;

        while (std::getline(queryStream, param, '&')) {
            size_t equalsPos = param.find('=');
            if (equalsPos != std::string::npos) {
                std::string key = param.substr(0, equalsPos);
                std::string value = param.substr(equalsPos + 1);
                params[key] = value;
            }
        }
    }
    return params;
}

std::string readFile(const std::string& path) {
    if (fileCache.count(path)) {
        return fileCache[path]; 
    }

    std::ifstream file(ROOT + path);
    if (!file) {
        return ""; 
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
    fileCache[path] = content; 
    return content;
}

void handleGetRequest(int clientSocket, const std::string& path) {
    std::string filePath = path;
    if (filePath == "/") {
        filePath = "/index.html"; 
    }

    std::string content = readFile(filePath);
    if (!content.empty()) {
        std::string extension = filePath.substr(filePath.find_last_of('.'));

        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: " + getMimeType(extension) + "\r\n";
        response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += content;

        send(clientSocket, response.c_str(), response.size(), 0);
    } else {
        std::string errorResponse = readFile("/404.html");
        if (errorResponse.empty()) {
            errorResponse = "HTTP/1.1 404 Not Found\r\n\r\n";
        } else {
            errorResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(errorResponse.size()) + "\r\n\r\n" + errorResponse;
        }
        send(clientSocket, errorResponse.c_str(), errorResponse.size(), 0);
    }
}


void handlePostRequest(int clientSocket, const std::string& request) {
    json responseJson = {
        {"status", "success"},
        {"message", "This is a response to a POST request."}
    };

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Content-Length: " + std::to_string(responseJson.dump().size()) + "\r\n";
    response += "Connection: close\r\n\r\n";
    response += responseJson.dump();

    send(clientSocket, response.c_str(), response.size(), 0);
}

void handleRequest(int clientSocket) {
    char buffer[1024];
    read(clientSocket, buffer, sizeof(buffer));
    
    std::string request(buffer);
    logRequest(request); 
    std::string method = request.substr(0, request.find(' '));
    std::string path = request.substr(request.find(' ') + 1);
    path = path.substr(0, path.find(' ')); 

    if (method == "GET") {
        handleGetRequest(clientSocket, path);
    } else if (method == "POST") {
        handlePostRequest(clientSocket, request);
    } else {
        std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}

int main() {
    loadConfig(); 

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(serverSocket, 5);
    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        handleRequest(clientSocket);
    }

    close(serverSocket);
    return 0;
}
