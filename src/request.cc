#include "include/request.h"

#include <sstream>
#include <iostream>
namespace httpLib{

    std::map<std::string, Method> methodMap = {
        {"GET", GET}, {"HEAD", HEAD}, {"POST", POST}, 
        {"PUT", PUT}, {"DELETE", DELETE}, {"CONNECT", CONNECT}, 
        {"OPTIONS", OPTIONS}, {"TRACE", TRACE}, {"PATCH", PATCH}
    };

    // Empty constructor
    Request::Request(){}
    
    // Constructor with request data
    Request::Request(char *recv_buf, const int bufferSize, int conn, int contentLength) {
        
        from = conn;
        if (recv_buf != nullptr && bufferSize > 0) {
            std::string requestData(recv_buf, bufferSize);
            std::istringstream requestStream(requestData);
            std::string line;
            bool headerSection = true;

            bool firstLineHandled = false;
            while (std::getline(requestStream, line)) {
                // Remove carriage return at the end if present
                if (!line.empty() && line[line.size() - 1] == '\r') {
                    line.erase(line.size() - 1);
                }

                if (headerSection) {
                    if (line.empty()) {
                        // Empty line: end of headers, start of content
                        headerSection = false;
                        continue;
                    }

                    if (!firstLineHandled)
                    {
                        std::istringstream iss(line);
                        std::string methodString;

                        iss >> methodString >> path >> httpVersion;

                        method = methodMap[methodString];
                        firstLineHandled = true;
                    }
                    

                    // Parse headers
                    size_t pos = line.find(": ");
                    if (pos != std::string::npos) {
                        std::string key = line.substr(0, pos);
                        std::string value = line.substr(pos + 2);
                        if (key == "user-agent") userAgent = value;
                        else if (key == "content-type") contentType = value;
                        else if (key == "Host") host = value;
                        else if (key == "Connection") connection = value;
                    }
                } else {
                    // Content section
                    content += line;
                    // If there are newlines in the content, they need to be handled accordingly
                }
            }
        }

        content = content.substr(0, contentLength);

        //split path
        std::vector<std::string> tokens;
        std::stringstream ss(this->path);
        std::string token;

        while (std::getline(ss, token, '/')) {
            if (!token.empty()) {  // Ignore empty tokens
                tokens.push_back(token);
            }
        }
        splitedPath = tokens;
    }

    std::string Request::toString(){
        std::stringstream ss;
        
        ss << "########################" << "\n"
           << "Method: " << methodToString(method) << "\n"
           << "Path: " << path << "\n"
           << "User-Agent: " << userAgent << "\n"
           << "Content-Type: " << contentType << "\n"
           << "Host: " << host << "\n"
           << "Connection: " << connection << "\n"
           << "Content: " << content << "\n"
           << "From: " << from;
        return ss.str();
    }

    std::string Request::methodToString(Method method){
        std::string methodString = "";
        for (auto e : methodMap){
            if(e.second == method){
                methodString = e.first;
            }
        }
        return methodString;
    }
}