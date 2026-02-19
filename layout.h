#pragma once
#include <vector>
#include "parser.h"
#include "raylib/include/raylib.h"

enum class displayType{
    displayBlock, displayInline
};

class layoutNode{
    public:
        std::vector<layoutNode*> children;
        layoutNode* parent;
        treeNode* originNode;

        float height;
        float width;
        float x;
        float y;

        float margin[4]; // top, bottom, right, left;
        float padding[4]; // top, bottom, right, left;
        float borders[4]; // top, bottom, right, left;
        
        displayType display;
        
        Color color;
        Color backgroundColor;
        Color borderColor;
};

class layoutTree{
    public:
        void makeLayoutTree(treeNode* node, layoutNode* parentLayout);
        void traverse(layoutNode* node, int level);

        float convertStringToPx(std::string input);

        layoutNode* layoutTreeRoot;

        // constants
        float emToPx = 16;
        float scale = 1;
};
