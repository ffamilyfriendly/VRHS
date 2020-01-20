/*
Copyright (c) 2020 ffamilyfriendly

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../headers/server.h"

void server::HttpServer::lsn(int port,int opt) {
    char buffer[1024] = {0}; 
    this->port = port;
    struct sockaddr_in adress;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    adress.sin_family = AF_INET;
    adress.sin_addr.s_addr = INADDR_ANY;
    adress.sin_port = htons(port);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    
    if(sock == 0) {
        perror("socket failed"); 
    }
     if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
    } 
        // Forcefully attaching socket to the port 8080 
    if (bind(sock, (struct sockaddr *)&adress,  
                                 sizeof(adress))<0) 
    { 
        perror("bind failed"); 
    } 


    int server_socket;

    std::string fofHTML = "<html><body>lol not found</body></html>";
    std::string fourOfour = "HTTP/1.0 404 Not Found\nContent-Type: text/html\nContent-Length: " + std::to_string(fofHTML.length()) + "\n\n" + fofHTML;

    if(listen(sock,5) < 0) {
        perror("cannot listen");
    } else {
        std::cout << "listening on port: " + std::to_string(port) << std::endl;
    }
    for(;;) {
        sin_size = sizeof their_addr;
        int client = accept(sock, (struct sockaddr *)&their_addr,&sin_size);
        if(client == -1) {
            perror("accept");
        }
        read(client,buffer,sizeof(buffer));

        server::request req(buffer);
        server::response res;
        res.socket = client;
        if(this->endpoints[req.path]) this->endpoints[req.path](res,req);
        else {
            res.snd(fourOfour);
            res.end();
        }
    }
}


