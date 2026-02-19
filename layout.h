#pragma once
#include <vector>
#include "parser.h"

class layoutNode{
    public:
        std::vector<layoutNode*> children;
        layoutNode* parent;
        treeNode* originNode;

        float height;
        float width;
        float x;
        float y;
};

class layoutTree{
    public:
        void makeLayoutTree(treeNode* node, layoutNode* parentLayout);
        void traverse(layoutNode* node, int level);

        layoutNode* layoutTreeRoot;
};
