#include "../headers/server.h"

void hello(server::response res,server::request req) {
    std::map<std::string,std::string> body = req.parseBody();
    //if no name cookie exists we redirect back to index page
    if(body["name"] == "") return res.redirect("/");
    //greet user
    res.renderFile("./greet.html",body);
}

void index(server::response res,server::request req) {
    res.sendFile("./index.html");
}

void regexItem(server::response res,server::request req) {
    res.sendFile(req.captured[1],"text/json");
}

/*
Example webserver that will ask for name and then greet user. 
*/

int main(int argc,  char** argv) {
    server::HttpServer *srv = new server::HttpServer();
    srv->endpoints["POST/hello"] = &hello;
    srv->endpoints["GET/"] = &index;
    //this endpoint will serve any file ending in .json with the url path /json/
    srv->endpoints["^GET/json/(.*\\.json)"] = &regexItem;
    srv->lsn(3000);
}