
#include "request.h"
#include "requestHandler.h"
#include "response.h"
#include "server.h"
#include <functional>

namespace httpLib {

    class httpServer{
        private:
            Server server;
            RequestHandler requestHandler;
            bool serverInitiliazed = false;
            bool serverRunning = false;

        public:

            httpServer(int port);
            void run();
            void stop();

            void GET(std::string path, std::function<Response(Request&)> method);
            void HEAD(std::string path, std::function<Response(Request&)> method);
            void POST(std::string path, std::function<Response(Request&)> method);
            void PUT(std::string path, std::function<Response(Request&)> method);
            void DELETE(std::string path, std::function<Response(Request&)> method);
            void CONNECT(std::string path, std::function<Response(Request&)> method);
            void OPTIONS(std::string path, std::function<Response(Request&)> method);
            void TRACE(std::string path, std::function<Response(Request&)> method);
            void PATCH(std::string path, std::function<Response(Request&)> method);

    };
}