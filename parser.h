#pragma once
#include <string>
#include <vector>

struct layoutData {
    float height;
    float width;
    float x;
    float y;
};

struct cssProperty{
    std::string name;
    std::string value;
    bool inheritable;
};

class treeNode{
    public:
        treeNode(std::string input, treeNode* parent);
        treeNode* parentNode;
        std::vector<treeNode*> children;
        std::string name;
        std::string data;
        std::vector<cssProperty> attributes;
        std::unique_ptr<layoutData> layout;
        ~treeNode();
};

class htmlParser{
    public:
        void parse(std::string input);
        void traverse(treeNode* node, int level);
        void inheritCss(treeNode* node);
        layoutData calculateLayout(treeNode* node);
        treeNode* domTree;
};
