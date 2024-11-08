#ifndef REQUEST_H
#define REQUEST_H

#include "string"
#include <map>
#include <vector>

namespace httpLib{

    enum Method { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH }; 
    
    class Request{

        public:
            Method method;
            std::string path;
            std::string httpVersion;
            std::string userAgent;
            std::string contentType;
            std::string host;
            std::string connection;
            std::string content;
            int from;

            std::map<std::string, std::string> pathParams;
            std::map<std::string, std::string> queryParams;
            std::vector<std::string> splitedPath;

            Request();
            Request(char *recv_buf, const int bufferSize, int conn, int contentLength);
            std::string toString();
            static std::string methodToString(Method method);

    };

}

#endif