#include "url.h"
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

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

std::string urlReader::request(){
    std::string content;

    struct addrinfo hints;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_family = PF_INET;
    hints.ai_flags = 0;

    int s = host.size();
    char node[s+1];
    strcpy(node, host.c_str());

    int p = scheme.size();
    char service[p+1];
    strcpy(service, scheme.c_str());

    struct addrinfo *result, *rp;

    int err = getaddrinfo(node, service, &hints, &result);
    int sfd;

    int i = 0;
    for(rp = result; rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd==-1) continue;
        if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) break;
    }

    char arr[1000];

    std::string request = "GET " + path +" HTTP/1.0\r\n" + "Host: " + host + "\r\n" + "\r\n";
    std::cout << request << std::endl;

    char req[request.size()+1];
    strcpy(req, request.c_str());

    write(sfd, req, sizeof(req));
    std::cout << req << std::endl;
    ::read(sfd, arr, sizeof(arr));
    std::cout << arr << std::endl;

    return content;
}
