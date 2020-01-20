# VRHS docs

VRHS (**V**ery **R**arted **H**ttp **S**erver) is a very simple and non-complex http server. It is designed to work like expressjs for node.


#### license: MIT
> Copyright (c) 2020 ffamilyfriendly

> Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

> The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


# client
### functions
* void lsn(int port) - starts listening for requests

### properties
* map<std::string,void (*)(response,request)> endpoints - map of endpoints

# request
### functions
* none

### properties
* std::string path;
* std::string type;
* std::map<std::string,std::string> headers - map of header values;
* std::map<std::string,std::string> query - map of query parameters;
* std::map<std::string,std::string> cookies - map of cookie values;

# response
### functions
* void snd(std::string data) - write string data 
* void end() - end socket connection (anything sent after this will cause an error)
* void sendFile(std::string path,std::string mime = "text/html") - send file
* void renderFile(std::string path,std::map<std::string,std::string> values) - like sendFile but replaces values formatted **--item--** to corresponding item in map
* void sendTC(std::string data) - like snd but closes connection after data is sent
* void status(short code) - set HTTP status code
* void redirect(std::string path) - redirect and close connection. **Note:** this overrides any previusly set status code