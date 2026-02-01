#pragma once
#include <iostream>

class urlReader{
    public: 
        std::string scheme;
        std::string host;
        std::string path;

        urlReader();
        bool read(std::string input);
};
