#include "parser.h"
#include <iostream>

enum readingState{
    reading, outside, readingend
};

void htmlParser::parse(std::string input){
    domTree = new treeNode("ROOT", nullptr);
    int state = outside;
    std::string word;
    std::string data;
    treeNode* curr = domTree;

    for(auto letter : input){
        if(letter=='<'){
            curr->data = data;
            state = reading;
            word = "";
            data = "";
        }else if(state == reading && word == "" && letter == '/'){
            curr = curr->parentNode;
            state = readingend;
        }else if(letter=='>' && state==reading){
            state = outside;
            treeNode* temp = new treeNode(word, curr);
            curr->children.emplace_back(temp);
            curr = temp;
        }else if(state==reading){
            word += letter;
        }else if(state == outside){
            data += letter;
        }
    }

}

void htmlParser::traverse(treeNode* node, int level){
    std::string indent;
    for(int i=0; i<level; i++){
        indent += '\t';
    }
    std::cout << indent << node->name << node->data << std::endl;
    for(auto child : node->children){
        traverse(child, level+1);
    }
}

treeNode::treeNode(std::string input, treeNode* parent){
    name = input;
    parentNode = parent;
}

treeNode::~treeNode(){
    for(auto child : children){
        delete child;
    }
}
