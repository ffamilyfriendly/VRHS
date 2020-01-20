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

//wriute data
void server::response::snd(std::string dta) {
    if(send(this->socket,dta.c_str(),dta.length(),0) == -1) {
        perror("snd()");
    };
}

//set status
void server::response::status(short stat) {
    this->code = stat;
}

//prepare headers. private function
void server::response::setHeaders() {
    std::string head;
    head += "HTTP/1.0 " + std::to_string(this->code) + "\n";
    for (std::pair<std::string, std::string> element : this->headers) {
		head += element.first + ": " + element.second + "\n";
	}
    head += "\n";
    this->snd(head);
}

//send data and close 
void server::response::sendTC(std::string data) {
    this->headers["Content-Type"] = "text/html";
    this->headers["Content-Length"] = std::to_string(data.length());
    this->setHeaders();
    this->snd(data);
    this->end();
}

//redirects to new thing
void server::response::redirect(std::string path) {
    this->status(301);
    this->headers["Location"] = path;
    this->setHeaders();
    this->end();
}

//ends the stream. attempt to write after this function is ran wil lresult in errors or smthn
void server::response::end() {
    if(close(this->socket) == -1) {
        perror("end()");
    };
}

//sends the file based on the path. simple
void server::response::sendFile(std::string pth, std::string mime) {
    //write HTTP status
    std::ifstream file(pth.c_str());
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    //set headers
    this->headers["Content-Type"] = mime;
    this->headers["Content-Length"] = std::to_string(str.length());
    this->setHeaders();
    this->snd(str);
    this->end();
}

//This function works like sendFile but replaces a token (--VARIABLENAME--) with that value from the passed map of strings
void server::response::renderFile(std::string path,std::map<std::string,std::string> values) {
    //REGEX
    std::regex tReg (
        R"(.*--(.*)--.*)",
        std::regex::extended
    );
    std::smatch regMatch;

    std::ifstream file(path);
    std::string line;
    std::string html;
    while(std::getline(file,line)) {
        std::regex_match(line,regMatch,tReg);
        std::string mtch = regMatch[1];
        if(mtch != "") {
            std::string::size_type where = line.find(mtch);
            std::string before = line.substr(0,where-2);
            std::string after = line.substr(where + mtch.length() + 2);
            line = before + values[mtch] + after;
        }
        html += line + "\n";
    }
    this->headers["Content-Type"] = "text/html";
    this->headers["Content-Length"] = std::to_string(html.length());
    this->setHeaders();
    this->snd(html);
    this->end();
}