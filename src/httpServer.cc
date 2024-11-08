#include "include/httpServer.h"
#include "include/requestHandler.h"
#include "include/request.h"
#include "include/response.h"

#include <sys/socket.h>     // for send
#include <unistd.h>         // for close

namespace httpLib {

    httpServer::httpServer(int port) : server(port), requestHandler() {
        this->serverInitiliazed = server.Init();
    }
    
    void httpServer::run(){

        if (!this->serverInitiliazed){
            std::cout << "httpLib Error: Cannot run the http server due to previous errors!" << std::endl;
            return;
        }
        
        this->serverRunning = true;
        
        while (this->serverRunning) {

            httpLib::Request req = server.Accept(); //infinite loop, it waits for a request
            
            httpLib::Response response = this->requestHandler.handle(req);
            send(req.from, response.response.c_str(), response.response.size(), 0);
            close(req.from);
        }

    }

    void httpServer::GET(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.getMethods[path] = method;
    }

    void httpServer::HEAD(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.headMethods[path] = method;
    }

    void httpServer::POST(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.postMethods[path] = method;
    }

    void httpServer::PUT(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.putMethods[path] = method;
    }

    void httpServer::DELETE(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.deleteMethods[path] = method;
    }

    void httpServer::CONNECT(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.connectMethods[path] = method;
    }

    void httpServer::OPTIONS(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.optionsMethods[path] = method;
    }

    void httpServer::TRACE(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.traceMethods[path] = method;
    }

    void httpServer::PATCH(std::string path, std::function<Response(Request&)> method){
        this->requestHandler.patchMethods[path] = method;
    }

    void httpServer::stop(){
        this->serverRunning = false;
    }
}