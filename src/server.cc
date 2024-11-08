#include "include/server.h"

#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "include/request.h"

namespace httpLib {

    Server::Server(int port){
        PORT = port;
    }

    bool Server::Init(){

        struct sockaddr_in server_addr;         // server info struct
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family=AF_INET;         // TCP/IP
        server_addr.sin_addr.s_addr=INADDR_ANY; // server addr--permit all connection
        server_addr.sin_port=htons(PORT);       // server port

        /* create socket fd with IPv4 and TCP protocal*/
        if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))<0) {  
            std::cout << "httpLib Error: Socket error!" << std::endl;
            return false;
        }

        // to unbind port if binded already
        int yes = 1;
        if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            std::cout << "httpLib Error: setsockopt SO_REUSEADDR error!" << std::endl;
            close(server_sockfd);
            return false;
        }

        /* bind socket with server addr */
        if(bind(server_sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))<0) {
            std::cout << "httpLib Error: Bind error!" << std::endl;
            return false;
        }

        /* listen connection request with a queue length of 20 */
        if(listen(server_sockfd,10)<0) {
            std::cout << "httpLib Error: Error while listening!" << std::endl;
            return false;
        }

        std::string listenMessage = "Listen on port " + std::to_string(PORT) + ".";
        std::cout << "httpLib INFO: " << listenMessage << std::endl;

        memset(recv_buf, '\0', sizeof(recv_buf));

        return true;
    }

    httpLib::Request Server::Accept(){

        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

        // block on accept until positive fd or error
        int conn = accept(server_sockfd, (struct sockaddr*)&client_addr,&length);
        
        //try-catch ?

        if(conn<0) {    //conn=-1 for errors
            std::cout << "httpLib Warn: Connection error!" << std::endl;
            httpLib::Request r;
            return r;
        }

        char client_ip[INET_ADDRSTRLEN] = "";
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

        httpLib::Request request;
        
        bool hasContent = true;
        char recv_buffer[4096];
        int totallyReceivedByteCount = 0;

        int receivedByteCount = recv(conn, recv_buffer, sizeof(recv_buffer), 0);

        for (int i = 0; i < receivedByteCount; i++)
        {
            recv_buf[totallyReceivedByteCount + i] = recv_buffer[i];
        }

        totallyReceivedByteCount += receivedByteCount;
        memset(recv_buffer, '\0', strlen(recv_buffer));

        /*
            In most case, http request sends with only one chunk.
            For multiple chunk request, check content-length of header
        */

        std::string firstChunk(recv_buf, receivedByteCount);

        size_t blankLinePos = firstChunk.find("\r\n\r\n");

        if (blankLinePos == std::string::npos) {
            std::cerr << "Invalid HTTP message.\n";
            return request; // empty request
        }

        int contentLength = -1;
        size_t contentLengthPos1 = firstChunk.find("content-length: ");
        size_t contentLengthPos2 = firstChunk.find("Content-Length: ");
        
        if (contentLengthPos1 == std::string::npos && contentLengthPos2 == std::string::npos) {
            hasContent = false;
        }
        else
        {
            size_t valueStart;
            if (contentLengthPos1 != std::string::npos)
            {
                valueStart = contentLengthPos1 + strlen("content-length: ");
            }
            else
            {
                valueStart = contentLengthPos2 + strlen("Content-Length: ");
            } 

            std::string lengthValueAndOtherPart = firstChunk.substr(valueStart);;
            
            contentLength = std::stoi(lengthValueAndOtherPart);
            
        }

        if (hasContent)
        {
            int bodyStart = int(blankLinePos) + int(strlen("\r\n\r\n"));
            
            if (bodyStart + contentLength != receivedByteCount)
            {

                int unreceivedByteCount = bodyStart + contentLength - receivedByteCount;

                if (unreceivedByteCount < 0)
                {
                    std::cerr << "content length problem" << std::endl;
                }

                while (unreceivedByteCount != 0)
                {
                    int byteCount;
                    if (unreceivedByteCount > 4096)
                    {
                        byteCount = 4096;
                    }
                    else
                    {
                        byteCount = unreceivedByteCount;
                    }

                    int receivedByteCount = recv(conn, recv_buffer, sizeof(recv_buffer), 0);

                    for (int i = 0; i < receivedByteCount; i++)
                    {
                        recv_buf[totallyReceivedByteCount + i] = recv_buffer[i];
                    }
                    memset(recv_buffer, '\0', strlen(recv_buffer));

                    totallyReceivedByteCount += receivedByteCount;

                    unreceivedByteCount -= byteCount;
                }
            }
            
        }
        
        httpLib::Request req(recv_buf, bufferSize, conn, contentLength);

        memset(recv_buf, '\0', strlen(recv_buf));
        
        return req;     // connnection open
    }
}