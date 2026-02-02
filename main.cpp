#include <iostream>
#include "url.h"
#include <sys/socket.h>

int main(){
    std::string test = "http://example.org";
    urlReader testReader;
    std::cout << testReader.read(test) << std::endl;
    testReader.request();
    return 0;
}
