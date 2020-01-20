#include "../headers/server.h"
/*
 Extremely simple example of a extremely simple HTTP server.
*/


void test(server::response res,server::request req) {
    res.sendFile("/home/jonathan/Documents/code/javascript/shitposting/boomer.html");
}

int main(int argc,  char** argv) {
    server::HttpServer *srv = new server::HttpServer();
    srv->endpoints["/"] = &test;
    srv->lsn(3000);
}