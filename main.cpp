#include <iostream>
#include "url.h"
#include <sys/socket.h>
#include "parser.h"

int main(){
    std::string test = "http://127.0.0.1/index.html";
    urlReader testReader;
    testReader.read(test);
    std::string header, body;
    testReader.request(header, body);

    htmlParser parser;

    parser.parse(body);

    parser.traverse(parser.domTree, 0);

    return 0;
}
