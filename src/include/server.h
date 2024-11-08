#ifndef SERVER_H
#define SERVER_H

#include "requestHandler.h"
#include "request.h"

namespace httpLib {

    class Server{

        private:
            int PORT;

        public:
            
            int server_sockfd;
            const int bufferSize = 65536;
            char recv_buf[65536];

            Server(int port);
            bool Init();
            httpLib::Request Accept();
        };

    }

#endif