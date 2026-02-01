#include <iostream>
#include "url.h"
#include <sys/socket.h>

int main(){
    std::string test = "https://google.com";
    urlReader testReader;
    std::cout << testReader.read(test) << std::endl;
    return 0;
}
