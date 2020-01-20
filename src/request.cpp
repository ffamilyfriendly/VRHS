#include "../headers/server.h"
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

server::request::request(char buffer[1024]) {
    std::istringstream resp(buffer);
    std::string header;
    std::string::size_type index;
    bool first = true;
    while(std::getline(resp,header) && header != "\r") {
        if(first) {
            index = header.find("/",0);
            std::string::size_type endOfPath = header.find("HTTP",index);
            std::string _path = header.substr(index,endOfPath-index-1);

            // main regex
            std::regex url_regex (
                R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
                std::regex::extended
            );

            std::smatch url_match_res;
            unsigned int count = 0;
            if(std::regex_match(_path,url_match_res,url_regex)) {
                this->path = url_match_res[5];
                this->type = header.substr(0,index-1);
                std::vector<std::string> _params = utils::split(url_match_res[7],'&');
                for(unsigned int i = 0; i < _params.size(); i++) {
                    std::string _param = _params[i];
                    std::string::size_type qs = _param.find("=");
                    this->query[_param.substr(0,qs)] = _param.substr(qs + 1);
                }
            }
            first = false;
        } else {
            index = header.find(":",0);
            if(index != std::string::npos) {
                this->headers[header.substr(0,index)] = header.substr(index + 1);
            }
        }
    }
    this->parseCookies();
}

void server::request::parseCookies() {
    std::vector<std::string> cookies = utils::split(this->headers["Cookie"],';');
    for(unsigned int i = 0; i < cookies.size(); i++) {
        std::string cookie = cookies[i];
        std::string::size_type cs = cookie.find("=");
        this->cookies[cookie.substr(1,cs-1)] = cookie.substr(cs+1);
    }
}