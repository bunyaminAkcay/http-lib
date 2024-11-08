# http-lib

http-lib is a simple, single-threaded HTTP library in C++17. It was developed for educational purposes and fun, and is not recommended for production use.

## usage

An example:

```c++
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
        std::string jsonString ="{\"result\": \"Hello " + request.pathParams["name"] +"!\"}";

        for (auto i : request.queryParams) {
            std::cout << "A query param:" << i.first << " " << i.second << std::endl;
        }

        httpLib::Response response(200, "application/json", jsonString);
        return response;
    });

    http.run();

    return 0;
}
```
## build examples

create build directory:
```bash
mkdir build
cd build
```
configure cmake:
```bash
cmake ../examples
```
build:
```bash
make
```
then run example-server1 or example-server2:
```bash
./example-server1
```