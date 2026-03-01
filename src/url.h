#pragma once
#include <iostream>
#include <curl/curl.h>

class urlReader{
    public: 
        std::string scheme;
        std::string host;
        std::string path;

        urlReader();
        bool read(std::string input);
        void request(std::string &header, std::string &body);
};

class curlReader{
    public:
        curlReader();
        ~curlReader();
        void fetch(std::string url, std::string& data);
    private:
        CURL *curl;
};


