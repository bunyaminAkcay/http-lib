#include "httpServer.h"
#include "request.h"
#include "response.h"

int main(){
    httpLib::httpServer http(8000);
    
    http.GET("/hello", [](httpLib::Request request){
        std::string content = "Hello";

        httpLib::Response response(200, "text/plain", content);
        return response;
    });

    http.GET("/hello/:name", [](httpLib::Request request){
        std::string jsonString = "{\"result\": \"Hello "+ request.pathParams["name"] +"!\"}";
        std::string contentType = "application/json";

        for (auto i : request.queryParams) {
            std::cout << "A query param:" << i.first << " " << i.second << std::endl;
        }

        httpLib::Response response(200, contentType, jsonString);
        return response;
    });

    http.run();

    return 0;
}