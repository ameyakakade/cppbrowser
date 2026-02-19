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

    /* filling data in layout nodes */

    auto fillArray = [&](float* array, const std::string& identifier){
        std::vector<std::string> suffix = {"-top", "-bottom", "-right", "-left"};
        for(int i=0; i<4; i++) array[i] = convertStringToPx(node->style[node->cssPropertyIndexCache[identifier+(suffix[i])]].value);
    };

    fillArray(currentLayoutNode->margin, "margin");
    fillArray(currentLayoutNode->padding, "padding");

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
    std::cout << indent << node->originNode->name << " ";

    std::cout << indent << " margin-top:" << node->margin[0] ;

    std::cout << "\n";
    for(auto child : node->children){
        traverse(child, level+1);
    }
}

float layoutTree::convertStringToPx(std::string& input){
    std::string data;
    std::string type;

    int state = 0;
    for(char c : input){
        if(state == 0){
            if( c>47 && c<58 || c==46){
                data += c;
            }else if(c == ' '){
                // do nothing
            }else{
                state = 1;
                type += c;
            }
        }else if(state == 1){
            if(c == ' ') break;
            type += c;
        }
    }

    float value;

    try{
        value = stof(data);
    }catch(std::invalid_argument){
        std::cout << "Invalid CSS in \"" << input << "\"" << std::endl;
        value = 16;
    }

    float multiplier = 1;

    if(type == "em") multiplier = emToPx;
    if(type == "px") multiplier = 1;

    return value*multiplier;
}
