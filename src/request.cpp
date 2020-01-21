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
    bool isHeader = true;
    while(std::getline(resp,header)) {
        if(header == "\r") isHeader = false;
        if(isHeader) {
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
                if(std::regex_match(_path,url_match_res,url_regex)) {
                    this->path = url_match_res[5];
                    this->type = header.substr(0,index-1);
                    std::vector<std::string> _params = utils::split(url_match_res[7],'&');
                    for(unsigned int i = 0; i < _params.size(); i++) {
                        std::vector<std::string> vl = utils::split(_params[i],'=');
                        this->query[vl[0]] = vl[1];
                    }
                }
                first = false;
            } else {
                std::vector<std::string> prts = utils::split(header,':');
                this->headers[prts[0]] = prts[1].substr(1);
            }
        } else if(header != "\r") this->data += header;
    }
    this->parseCookies();
}

void server::request::parseCookies() {
    std::vector<std::string> cookies = utils::split(this->headers["Cookie"],';');
    for(unsigned int i = 0; i < cookies.size(); i++) {
        std::string cookie = cookies[i];
        std::vector<std::string> cki = utils::split(cookie,'='); 
        this->cookies[cki[0]] = cki[1];
    }
}

std::map<std::string,std::string> server::request::parseBody() {
    std::map<std::string,std::string> body;
    std::vector<std::string> values = utils::split(this->data,'&');
    for(unsigned int i = 0; i < values.size(); i++) {
        std::vector<std::string> k_v = utils::split(values[i],'=');
        body[k_v[0]] = k_v[1];
    }
    return body;
}