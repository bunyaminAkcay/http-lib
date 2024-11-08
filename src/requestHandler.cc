#include "include/requestHandler.h"

#include "string"
#include <iostream>
#include <ostream>

namespace httpLib{

    Response RequestHandler::notFound(Request& request){
        std::string notFoundString = "Not found!";

        std::string jsonString = "{\"result\": \"" + notFoundString + "\"}";
        std::string contentType = "application/json";
        Response response(404, contentType, jsonString);
        return response;
    }

    RequestHandler::RequestHandler(){}
    
    Response RequestHandler::handleGet(Request& request){


        for (auto const& pair : this->getMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handleHead(Request& request){
        
        for (auto const& pair : this->headMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handlePost(Request& request){
        
        for (auto const& pair : this->postMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handlePut(Request& request){
        
        for (auto const& pair : this->putMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handleDelete(Request& request){
        
        for (auto const& pair : this->deleteMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }
    
    Response RequestHandler::handleConnect(Request& request){
        
        for (auto const& pair : this->connectMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handleOptions(Request& request){
        
        for (auto const& pair : this->optionsMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handleTrace(Request& request){
        
        for (auto const& pair : this->traceMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handlePatch(Request& request){
        
        for (auto const& pair : this->patchMethods)
        {
            if (checkPathIsEqual(request, pair.first)){
                return pair.second(request);
            }
        }

        return notFound(request);
    }

    Response RequestHandler::handle(Request& request){

        std::cout <<"httpLib INFO: Received " << Request::methodToString(request.method) << " " << request.path << " from " << request.host << std::endl;

        if (request.method == Method::GET)
        {
            return handleGet(request);
        }
        else if(request.method == Method::HEAD){
            return handleHead(request);
        }
        else if(request.method == Method::POST){
            return handlePost(request);
        }
        else if(request.method == Method::PUT){
            return handlePut(request);
        }
        else if(request.method == Method::DELETE){
            return handleDelete(request);
        }
        else if(request.method == Method::CONNECT){
            return handleConnect(request);
        }
        else if(request.method == Method::OPTIONS){
            return handleOptions(request);
        }
        else if(request.method == Method::TRACE){
            return handleTrace(request);
        }
        else if(request.method == Method::PATCH){
            return handlePatch(request);
        }
        
        return notFound(request);
    }

    bool RequestHandler::checkPathIsEqual(Request& request, std::string path){
        std::vector<std::string> splitedPath;
        std::stringstream ss(path);
        std::string token;

        while (std::getline(ss, token, '/')) {
            if (!token.empty()) {  // Ignore empty tokens
                splitedPath.push_back(token);
            }
        }

        if (splitedPath.size() != request.splitedPath.size()) {
            return false;
        }

        bool samePath = true;  

        for ( int i = 0; i < request.splitedPath.size() ; i++){
            if (splitedPath[i][0] == ':'){
                continue;
            }

            size_t pos1 = request.splitedPath[i].find('?');
            std::string token1 = (pos1 != std::string::npos) ? request.splitedPath[i].substr(0, pos1) : request.splitedPath[i];

            size_t pos2 = splitedPath[i].find('?');
            std::string token2 = (pos2 != std::string::npos) ? splitedPath[i].substr(0, pos2) : splitedPath[i];

            if (token1 != token2) {
                samePath = false;
                break;
            }
        }

        if (!samePath) {
            return false;
        }

        for ( int i = 0; i < request.splitedPath.size() ; i++){

            size_t pos = request.splitedPath[i].find('?');
            std::string token = (pos != std::string::npos) ? request.splitedPath[i].substr(0, pos) : request.splitedPath[i];
            
            if (splitedPath[i][0] == ':'){
                request.pathParams[splitedPath[i].substr(1)] = token;
            }

            std::string remainingPart = "";
            if (pos != std::string::npos) {
                remainingPart = request.splitedPath[i].substr(pos+1);
            }
            
            std::istringstream ss(remainingPart);
            std::string pair;
            
            while (std::getline(ss, pair, '&')) {
                size_t pos = pair.find('=');
                
                if (pos != std::string::npos) {
                    std::string key = pair.substr(0, pos);
                    std::string value = pair.substr(pos + 1);
                    
                    request.queryParams[key] = value;
                }
            }

        }

        return true;
    }

}