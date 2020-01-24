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

//includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>

//defines
#define version_major 1
#define version_minor 0
#define version_patch 0

namespace server {
    struct sockaddr_in{
    short   sin_family; /* must be AF_INET */
    u_short sin_port;
    struct  in_addr sin_addr;
    char    sin_zero[8]; /* Not used, must be zero */
    };

    class response {
        public:
        //HTML headers (also shameless plug)
        std::map<std::string,std::string> headers = {{"x-served-by","github.com/ffamilyfriendly/VRHS"}};
        int socket;
        std::string data;
        void snd(std::string data);
        void end();
        void sendFile(std::string path,std::string mime = "text/html");
        void renderFile(std::string path,std::map<std::string,std::string> values);
        void sendTC(std::string data);
        void status(short code);
        void redirect(std::string path);
        private:
        void setHeaders();
        short code = 200;
    };

    class request {
        public:
        request(char buffer[1024]);
        void parseCookies();
        std::map<std::string,std::string> parseBody();
        std::string path;
        std::string type;
        std::string data;
        std::smatch captured;
        std::map<std::string,std::string> headers;
        std::map<std::string,std::string> query;
        std::map<std::string,std::string> cookies;
    };

    class HttpServer {
        public:
        //defualt read buffer size is 34kb
        void lsn(int port,int opt = 1, int buffSize = 1024 * 34); 
        std::map<std::string,void (*)(response,request)> endpoints;
        private:
        int port;
    };
}

namespace utils {
    std::vector<std::string> split(const std::string &s, char delim);
}