#include "../headers/server.h"

void hello(server::response res,server::request req) {
    std::map<std::string,std::string> e = {{"name",req.cookies["name"]}};
    //if no name cookie exists we redirect back to index page
    if(req.cookies["name"] == "") return res.redirect("/");
    //greet user
    res.renderFile("./greet.html",e);
}

void greetings(server::response res,server::request req) {
    //if no name param exists we redirect back to index page
    if(req.query["name"] == "") return res.redirect("/");
    res.headers["Set-Cookie"] = "name =" + req.query["name"];
    //send to hello page
    res.redirect("/hello");
}

void index(server::response res,server::request req) {
    res.sendFile("./index.html");
}

/*
Example webserver that will ask for name and then greet user. 
*/

int main(int argc,  char** argv) {
    server::HttpServer *srv = new server::HttpServer();
    srv->endpoints["GET/hello"] = &hello;
    srv->endpoints["GET/greetings"] = &greetings;
    srv->endpoints["GET/"] = &index;
    srv->lsn(3000);
}