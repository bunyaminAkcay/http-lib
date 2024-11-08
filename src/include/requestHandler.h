#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <map>
#include <functional>

#include "request.h"
#include "response.h"

namespace httpLib{
    
    class RequestHandler{
        public:
             
            std::map<std::string, std::function<Response(Request&)>> getMethods;
            std::map<std::string, std::function<Response(Request&)>> headMethods;
            std::map<std::string, std::function<Response(Request&)>> postMethods;
            std::map<std::string, std::function<Response(Request&)>> putMethods;
            std::map<std::string, std::function<Response(Request&)>> deleteMethods;
            std::map<std::string, std::function<Response(Request&)>> connectMethods;
            std::map<std::string, std::function<Response(Request&)>> optionsMethods;
            std::map<std::string, std::function<Response(Request&)>> traceMethods;
            std::map<std::string, std::function<Response(Request&)>> patchMethods;

            RequestHandler();
            Response handle(Request& request);
            
        private:

            Response notFound(Request& request);

            Response handleGet(Request& request);
            Response handleHead(Request& request);
            Response handlePost(Request& request);
            Response handlePut(Request& request);
            Response handleDelete(Request& request);
            Response handleConnect(Request& request);
            Response handleOptions(Request& request);
            Response handleTrace(Request& request);
            Response handlePatch(Request& request);

            //check paths are equal, if it is, add request params
            bool checkPathIsEqual(Request& request, std::string path);
    };
}

#endif