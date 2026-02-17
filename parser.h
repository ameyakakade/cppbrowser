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

struct attributes{
    std::string name;
    std::string value;
};

struct defaults{
    std::vector<cssProperty> properties;
};

class treeNode{
    public:
        treeNode(std::string input, treeNode* parent);
        treeNode* parentNode;
        std::vector<treeNode*> children;
        std::string name;
        std::string text;
        std::string rawAttributes;
        std::vector<cssProperty> style;
        std::vector<attributes> nodeAttributes;
        std::unique_ptr<layoutData> layout;
        ~treeNode();
};

class htmlParser{
    public:
        void parse(std::string input);
        void parseAttributes(treeNode* node);
        void inheritCss(treeNode* node);
        layoutData calculateLayout(treeNode* node);
        void traverse(treeNode* node, int level);

        treeNode* domTree;
};
