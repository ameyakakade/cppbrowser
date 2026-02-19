#include "layout.h"
#include <iostream>
#include <string>

// body and root node
void layoutTree::makeLayoutTree(treeNode* node, layoutNode* parentLayout){

    // create a layout node
    layoutNode* currentLayoutNode = new layoutNode;

    // add its parent layout node
    currentLayoutNode->parent = parentLayout;

    // link it to its origin dom node
    currentLayoutNode->originNode = node;

    // add it to children of parent layout node
    parentLayout->children.push_back(currentLayoutNode);

    // now we have to recurse and add layout nodes for all children of the treenode which will be linked to current layout node
    for(auto child : node->children){
        makeLayoutTree(child, currentLayoutNode);
    }

}

void layoutTree::traverse(layoutNode* node, int level){
    std::string indent;
    for(int i=0; i<level; i++){
        indent += "   ";
    }
    std::cout << indent << node->originNode->name << std::endl;
    std::cout << "\n";
    for(auto child : node->children){
        traverse(child, level+1);
    }
}
