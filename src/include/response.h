#ifndef RESPONSE_H
#define RESPONSE_H

#include "string"
#include <iostream>
#include <sstream>
#include <map>

namespace httpLib{


    class Response{
        
        public:
            std::string response;
            std::string content;
            int statusCode;
            std::string contentType;

            Response();
            Response(int statusCode, std::string contentType, std::string content);
    };

}

#endif