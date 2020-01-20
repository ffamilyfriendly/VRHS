#include "../headers/server.h"

std::vector<std::string> &spl(const std::string &s, char delim,std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (item.length() > 0) {
            elems.push_back(item);  
        }
    }
    return elems;
}

std::vector<std::string> utils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    spl(s, delim, elems);
    return elems;
}