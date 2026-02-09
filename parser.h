#pragma once
#include <string>
#include <vector>

class treeNode{
    public:
        treeNode(std::string input, treeNode* parent);
        treeNode* parentNode;
        std::vector<treeNode*> children;
        std::string name;
        std::string data;
        ~treeNode();
};

class htmlParser{
    public:
        void parse(std::string input);
        void traverse(treeNode* node, int level);
        treeNode* domTree;
};

