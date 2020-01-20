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

void server::response::snd(std::string dta) {
    if(send(this->socket,dta.c_str(),dta.length(),0) == -1) {
        perror("snd()");
    };
}

void server::response::setHeaders() {
    std::string head;
    for (std::pair<std::string, std::string> element : this->headers) {
		head += element.first + ": " + element.second + "\n";
	}
    head += "\n";
    this->snd(head);
}

void server::response::end() {
    if(close(this->socket) == -1) {
        perror("end()");
    };
}

void server::response::sendFile(std::string pth) {
    //write HTTP status
    this->snd("HTTP/1.0 200 OK\n");
    std::ifstream file(pth.c_str());
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    //set headers
    this->headers["Content-Type"] = "text/html";
    this->headers["Content-Length"] = std::to_string(str.length());
    this->setHeaders();
    this->snd(str);
    this->end();
}