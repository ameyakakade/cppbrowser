#include "url.h"
#include <cctype>

urlReader::urlReader(){

}

bool urlReader::read(std::string input){
    size_t colon_pos = input.find(":");

    if(colon_pos==std::string::npos) return false;

    if(input.size()<colon_pos+2) return false;
    if(input[colon_pos+1]!='/') return false;
    if(input[colon_pos+2]!='/') return false;

    scheme = input.substr(0, colon_pos);
    std::string temp = input.substr(colon_pos+3);

    size_t path_pos = temp.find("/");

    host = temp.substr(0, path_pos);
    if(path_pos==std::string::npos) path="/";
    else path = temp.substr(path_pos);

    for(char& c : scheme){
        c = std::tolower((unsigned char)c);
    }
    
    if(scheme=="http"){
        std::cout << "http detected" << std::endl;
    }

    return true;
}
